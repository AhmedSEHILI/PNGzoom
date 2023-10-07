#include<stdlib.h>
#include<stdio.h>
#include"lodepng.h"



struct myimage{
	unsigned height;
	unsigned width;
	unsigned char* canalRouge;
	unsigned char* canalVert;
	unsigned char* canalBlue;
	unsigned char* canalAlpha;
};


struct myimage LireImage(char* nom_fichier){

	  struct myimage myPng;
	
	  unsigned error;
	  unsigned char* image;
	  unsigned width, height;

	  error = lodepng_decode32_file(&image, &width, &height, nom_fichier);
	  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));

	  myPng.width = width;
	  myPng.height = height;
	  
	  myPng.canalRouge = malloc(width * height);
	  myPng.canalVert = malloc(width * height);
	  myPng.canalBlue = malloc(width * height);
	  myPng.canalAlpha = malloc(width * height);	

	  
	  int i, saut = 0;
	  
	  for(i = 0; i < width*height; i++){
	      	myPng.canalRouge[i] =  image[saut];
	      	myPng.canalVert[i] =  image[saut+1];
	      	myPng.canalBlue[i] =  image[saut+2];
	      	myPng.canalAlpha[i] =  image[saut+3];
	      	saut +=4;	
	      
	  }

	  free(image);
	
return myPng;
}


void EcrireImage(struct myimage im, char* nom_fichier){

	  unsigned char* image = malloc((im.width)*(im.height)*4);

	  int i, saut = 0;
	  
	  for(i = 0; i < (im.width)*(im.height); i++){
	  	image[saut] = im.canalRouge[i];
	  	image[saut+1] = im.canalVert[i];
	  	image[saut+2] = im.canalBlue[i];
	  	image[saut+3] = im.canalAlpha[i];
	  	
	  	saut +=4;
	  }
	  
		

  	  unsigned error = lodepng_encode32_file(nom_fichier, image, im.width, im.height);


 	  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));

}





/***************************************************************************************/
/***************************************************************************************/

struct maspline {
	double *x;
	 
	double *a;
	double *b;
	double *c;
	double *d;
	
	int n;

};

void calcul_une_spline(double X, double Y, double NextX,double NextY, double k1, double k2, double *a, double *b, double *c, double *d){
	
	double e = (-2)*NextY + 2*Y + k1 + k2;
	double f = 3*NextY - 3*Y - 2*k1 - k2;
	
	double d3 = (NextX - X)*(NextX - X)*(NextX - X);
	double d2 = (NextX - X)*(NextX - X);
	double d1 = (NextX - X);
	
    double t1, t2, t3;

    t1 = (((-1)*e)/d3)*X*X;
    t2 = (f/d2)*X*X;
    t3 = (k1/d1)*X;

	*a = e/d3;
	*b = ((((-3)*e) / d3)*X ) + (f/d2);
	*c = (((3*e)/d3)*X*X) - (((2*f)/d2)*X) + (k1/d1);
	*d = t1*X + t2 - t3 + Y;

	
	//printf("%f  ,  %f  ,  %f  ,  %f  ,\n",*a, *b, *c, *d);
	
}


struct maspline calcul_spline(double *tab_x, double *tab_y, int n){
	
	struct maspline spline; 
	spline.x = (double*)malloc(sizeof(double*)*n);
	spline.a = (double*)malloc(sizeof(double*)*(n-1));
	spline.b = (double*)malloc(sizeof(double*)*(n-1));
	spline.c = (double*)malloc(sizeof(double*)*(n-1));
	spline.d = (double*)malloc(sizeof(double*)*(n-1));
	spline.n = n;
	
	int i;
	double k1, k2;
	for (i=0; i<n; i++){
		spline.x[i] = tab_x[i];
	}
	
	k1 = (tab_y[1] - tab_y[0]) / (tab_x[1] - tab_x[0]);
	k2 = (tab_y[2] - tab_y[0]) / (tab_x[2] - tab_x[0]);
	
	calcul_une_spline(tab_x[0],tab_y[0], tab_x[1], tab_y[1], k1, k2, &spline.a[0], &spline.b[0], &spline.c[0], &spline.d[0]);

   
	for (i=1; i<n-2; i++){  // printf("%d \n", i);
        //printf("%f  |  %f\n", tab_y[i], tab_y[i+1]);
		k1 = (tab_y[i+1] - tab_y[i-1]) / (tab_x[i+1] - tab_x[i-1]);
        k2 = (tab_y[i+2] - tab_y[i]) / (tab_x[i+2] - tab_x[i]);

	    calcul_une_spline(tab_x[i],tab_y[i], tab_x[i+1], tab_y[i+1], k1, k2, &spline.a[i], &spline.b[i], &spline.c[i], &spline.d[i]);
        
        //printf("%d  |  %f  ,  %f  ,  %f  ,  %f  ,\n", i, spline.a[i], spline.b[i], spline.c[i], spline.d[i]);
	} 

    k1 = (tab_y[n-1] - tab_y[n-3]) / (tab_x[n-1] - tab_x[n-3]);
    k2 = (tab_y[n-2] - tab_y[n-3]) / (tab_x[n-2] - tab_x[n-3]);

	    calcul_une_spline(tab_x[n-2],tab_y[n-2], tab_x[n-1], tab_y[n-1], k1, k2, &spline.a[n-2], &spline.b[n-2], &spline.c[n-2], &spline.d[n-2]);


	return spline;


}

/**********************************************************************/
/**********************************************************************/


struct myimage elargir_image(struct myimage petite_image){

    struct myimage image;
    int i, indice_colonne = 0, indice_ligne = 0, indice_petite_image = 0;

    image.width = 3*petite_image.width - 2;    //  = n + 2(n-1)
    image.height = 3*petite_image.height - 2;
    
    image.canalRouge = (unsigned char*)malloc(sizeof(unsigned char*)*image.width*image.height);
    image.canalVert = (unsigned char*)malloc(sizeof(unsigned char*)*image.width*image.height);
    image.canalBlue = (unsigned char*)malloc(sizeof(unsigned char*)*image.width*image.height);
    image.canalAlpha = (unsigned char*)malloc(sizeof(unsigned char*)*image.width*image.height); 

    for (i = 0; i < image.width*image.height; i++){
        if ((indice_ligne%3 == 0) && (indice_colonne%3 == 0)){
                //printf("%d  - vrai\n", i);
                image.canalRouge[i] = petite_image.canalRouge[indice_petite_image];
                image.canalVert[i] = petite_image.canalVert[indice_petite_image];
                image.canalBlue[i] = petite_image.canalBlue[indice_petite_image];
                image.canalAlpha[i] = petite_image.canalAlpha[indice_petite_image];

                indice_petite_image++;
            
        }
        else{
                image.canalRouge[i] = 0;
                image.canalVert[i] = 0;
                image.canalBlue[i] = 0;
                image.canalAlpha[i] = 0;

                

        }

        indice_colonne++;
        if (indice_colonne==image.width){

           indice_colonne = 0;
           indice_ligne++;
        }
    }
    

return image;
}

void push(double *tab, double val, int actual){

    tab[actual] = val;

}



void interpolerLignes(struct myimage* image_large){

    struct maspline splineRed, splineGreen, splineBlue, splineAlpha;	


    double *tab_x, *tab_y_red, *tab_y_green, *tab_y_blue, *tab_y_alpha;
    int i, indice, indice_colonne = 0, indice_ligne = 0;

    int n = ((image_large->width + 2)/3);


    tab_y_red = (double*)malloc(sizeof(double*)*n);  
    tab_y_green = (double*)malloc(sizeof(double*)*n);
    tab_y_blue = (double*)malloc(sizeof(double*)*n);            
    tab_y_alpha = (double*)malloc(sizeof(double*)*n);
    tab_x = (double*)malloc(sizeof(double*)*n); // n = (t+2)/3
    

    int j, polynome;

   // on peut remplir le tableau des x une seule fois
    
    i = 0; indice = 0;

    while(i< (image_large->width)){
            tab_x[indice] = (double)i; 
            indice++;
            i = i + 3;           
    }


    int i_interp;

    indice = 0;
    i = 0;
    while (i < image_large->width*image_large->width){
        if ((indice_ligne%3 == 0) && (indice_colonne%3 == 0)){
            push(tab_y_red, image_large->canalRouge[i], indice);
            push(tab_y_green, image_large->canalVert[i], indice); 
            push(tab_y_blue, image_large->canalBlue[i], indice);
            push(tab_y_alpha, image_large->canalAlpha[i], indice);      
            indice++; 

        }

        indice_colonne++;

        if (indice_colonne == image_large->width){


            splineRed = calcul_spline(tab_x, tab_y_red, n);
            splineGreen = calcul_spline(tab_x, tab_y_green, n);
            splineBlue = calcul_spline(tab_x, tab_y_blue, n);
            splineAlpha = calcul_spline(tab_x, tab_y_alpha, n);

            i_interp = i - image_large->width + 1;
            polynome = 0; j =0;
            while (j<(image_large->width)){
                    if ((indice_ligne%3 == 0)&&(j%3 != 0)){
                        image_large->canalRouge[i_interp] = splineRed.a[polynome]*j*j*j + splineRed.b[polynome]*j*j + splineRed.c[polynome]*j +splineRed.d[polynome];
                        image_large->canalRouge[i_interp+1] = splineRed.a[polynome]*(j+1)*(j+1)*(j+1) + splineRed.b[polynome]*(j+1)*(j+1) + splineRed.c[polynome]*(j+1) +splineRed.d[polynome];


                        image_large->canalVert[i_interp] = splineGreen.a[polynome]*j*j*j + splineGreen.b[polynome]*j*j + splineGreen.c[polynome]*j +splineGreen.d[polynome];
                        image_large->canalVert[i_interp+1] = splineGreen.a[polynome]*(j+1)*(j+1)*(j+1) + splineGreen.b[polynome]*(j+1)*(j+1) + splineGreen.c[polynome]*(j+1) +splineGreen.d[polynome];


                        image_large->canalBlue[i_interp] = splineBlue.a[polynome]*j*j*j + splineBlue.b[polynome]*j*j + splineBlue.c[polynome]*j +splineBlue.d[polynome];
                        image_large->canalBlue[i_interp+1] = splineBlue.a[polynome]*(j+1)*(j+1)*(j+1) + splineBlue.b[polynome]*(j+1)*(j+1) + splineBlue.c[polynome]*(j+1) +splineBlue.d[polynome];


                        image_large->canalAlpha[i_interp] = splineAlpha.a[polynome]*j*j*j + splineAlpha.b[polynome]*j*j + splineAlpha.c[polynome]*j +splineAlpha.d[polynome];
                        image_large->canalAlpha[i_interp+1] = splineAlpha.a[polynome]*(j+1)*(j+1)*(j+1) + splineAlpha.b[polynome]*(j+1)*(j+1) + splineAlpha.c[polynome]*(j+1) +splineAlpha.d[polynome];



                        i_interp ++;
                        j++;
                        polynome ++;
                    }
                    i_interp ++;
                    j ++;

                    
            }

                      

            /*for(j = 0; j<n; j++){

                printf("%d | %f\n",i, tab_y_red[j]);
            }*/
            indice_colonne = 0;
            indice_ligne++;
            indice = 0;

  
         //   printf("\n--------------------\n");
        }

        i++;
    }

free(tab_x); free(tab_y_red); free(tab_y_green); free(tab_y_blue); free(tab_y_alpha);
}



void interpolerColonnes(struct myimage* image_large){

    struct maspline splineRed, splineGreen, splineBlue, splineAlpha;	

    int n, i, indice, j, indice_colonne, polynome;
    double  *tab_x2, *tab_y_blue2, *tab_y_red2,  *tab_y_green2, *tab_y_alpha2;
    

    n = ((image_large->height + 2)/3); 

    tab_x2 = (double*)malloc(sizeof(double*)*n);
    tab_y_red2 = (double*)malloc(sizeof(double*)*n);  
    tab_y_green2 = (double*)malloc(sizeof(double*)*n);
    tab_y_blue2 = (double*)malloc(sizeof(double*)*n);            
    tab_y_alpha2 = (double*)malloc(sizeof(double*)*n);

    i = 0; indice = 0;
    while(i< (image_large->height)){
            tab_x2[indice] = (double)i; 
            indice++;
            i = i + 3;           
    }

    j = 0;

    while (j < image_large->width){  

        i = j; indice = 0; indice_colonne = 0; polynome = 0;
        while (i < image_large->width*image_large->height){

            if ((j > 0) && (i > image_large->width)){                      //remplir les vides sans ajouter une nouvelle boucle


                image_large->canalRouge[i - (image_large->width)-1] = splineRed.a[polynome]*(indice_colonne-2)*(indice_colonne-2)*(indice_colonne-2) + splineRed.b[polynome]*(indice_colonne-2)*(indice_colonne-2) +    splineRed.c[polynome]*(indice_colonne-2) +splineRed.d[polynome];

                image_large->canalRouge[i - 2*(image_large->width)-1] = splineRed.a[polynome]*(indice_colonne-1)*(indice_colonne-1)*(indice_colonne-1) + splineRed.b[polynome]*(indice_colonne-1)*(indice_colonne-1) + splineRed.c[polynome]*(indice_colonne-1) +splineRed.d[polynome];

                image_large->canalVert[i - (image_large->width)-1] = splineGreen.a[polynome]*(indice_colonne-2)*(indice_colonne-2)*(indice_colonne-2) + splineGreen.b[polynome]*(indice_colonne-2)*(indice_colonne-2) + splineGreen.c[polynome]*(indice_colonne-2) +splineGreen.d[polynome];

                image_large->canalVert[i - 2*(image_large->width)-1] = splineGreen.a[polynome]*(indice_colonne-1)*(indice_colonne-1)*(indice_colonne-1) + splineGreen.b[polynome]*(indice_colonne-1)*(indice_colonne-1) + splineGreen.c[polynome]*(indice_colonne-1) +splineGreen.d[polynome];


                image_large->canalBlue[i - (image_large->width)-1] = splineBlue.a[polynome]*(indice_colonne-2)*(indice_colonne-2)*(indice_colonne-2) + splineBlue.b[polynome]*(indice_colonne-2)*(indice_colonne-2) + splineBlue.c[polynome]*(indice_colonne-2) + splineBlue.d[polynome]; 


                image_large->canalBlue[i - 2*(image_large->width)-1] = splineBlue.a[polynome]*(indice_colonne-1)*(indice_colonne-1)*(indice_colonne-1) + splineBlue.b[polynome]*(indice_colonne-1)*(indice_colonne-1) + splineBlue.c[polynome]*(indice_colonne-1) +splineBlue.d[polynome];

image_large->canalAlpha[i - (image_large->width)-1] = splineAlpha.a[polynome]*(indice_colonne-2)*(indice_colonne-2)*(indice_colonne-2) + splineAlpha.b[polynome]*(indice_colonne-2)*(indice_colonne-2) + splineAlpha.c[polynome]*(indice_colonne-2) +splineAlpha.d[polynome];

                image_large->canalAlpha[i - 2*(image_large->width)-1] = splineAlpha.a[polynome]*(indice_colonne-1)*(indice_colonne-1)*(indice_colonne-1) + splineAlpha.b[polynome]*(indice_colonne-1)*(indice_colonne-1) + splineAlpha.c[polynome]*(indice_colonne-1) +splineAlpha.d[polynome];

                polynome ++;


          
            }
            push(tab_y_red2, image_large->canalRouge[i], indice);
            push(tab_y_green2, image_large->canalVert[i], indice);
            push(tab_y_blue2, image_large->canalBlue[i], indice);
            push(tab_y_alpha2, image_large->canalAlpha[i], indice);


            i = i + 3*(image_large->width);
            indice_colonne = indice_colonne + 3;  

            indice++;

        }
        //printf("\n%d", j);
       
        splineRed = calcul_spline(tab_x2, tab_y_red2, n);
        splineGreen = calcul_spline(tab_x2, tab_y_green2, n);
        splineBlue = calcul_spline(tab_x2, tab_y_blue2, n);
        splineAlpha = calcul_spline(tab_x2, tab_y_alpha2, n);            
        
                                                          //dans la boucle suivante on remplit les vides de la colonne précédente
                                                          // ainsi on évite une autre boucle
                                                          // or la derinière colonne contient des vides en fin de boucle exterieure

   
        j++;
    }
    

    // ici ,on fixe le problème de la dernière colonne, sa spline est deja prete de la boucle précedente

    i = 2*image_large->width - 1; j = 1; polynome = 0;
    while (j < image_large->height){

                        image_large->canalRouge[i] = splineRed.a[polynome]*j*j*j + splineRed.b[polynome]*j*j + splineRed.c[polynome]*j +splineRed.d[polynome];
                        image_large->canalRouge[i + image_large->width] = splineRed.a[polynome]*(j+1)*(j+1)*(j+1) + splineRed.b[polynome]*(j+1)*(j+1) + splineRed.c[polynome]*(j+1) +splineRed.d[polynome];


 image_large->canalVert[i] = splineGreen.a[polynome]*j*j*j + splineGreen.b[polynome]*j*j + splineGreen.c[polynome]*j +splineGreen.d[polynome];
                        image_large->canalVert[i + image_large->width] = splineGreen.a[polynome]*(j+1)*(j+1)*(j+1) + splineGreen.b[polynome]*(j+1)*(j+1) + splineGreen.c[polynome]*(j+1) +splineGreen.d[polynome];


 image_large->canalBlue[i] = splineBlue.a[polynome]*j*j*j + splineBlue.b[polynome]*j*j + splineBlue.c[polynome]*j +splineBlue.d[polynome];
                        image_large->canalBlue[i + image_large->width] = splineBlue.a[polynome]*(j+1)*(j+1)*(j+1) + splineBlue.b[polynome]*(j+1)*(j+1) + splineBlue.c[polynome]*(j+1) +splineBlue.d[polynome];

 image_large->canalAlpha[i] = splineAlpha.a[polynome]*j*j*j + splineAlpha.b[polynome]*j*j + splineAlpha.c[polynome]*j +splineAlpha.d[polynome];
                        image_large->canalAlpha[i + image_large->width] = splineAlpha.a[polynome]*(j+1)*(j+1)*(j+1) + splineAlpha.b[polynome]*(j+1)*(j+1) + splineAlpha.c[polynome]*(j+1) +splineAlpha.d[polynome];
        

        
        i = i + 3*image_large->width;
        j = j+3;
        polynome ++;

    }

free(tab_x2); free(tab_y_red2); free(tab_y_green2); free(tab_y_blue2); free(tab_y_alpha2);
    
}





int main(int argc, char *argv[]){


    struct myimage m, m2;



	if (argc != 3){
		printf("Le nombre des entrées du programme est incorrect\n");
		return EXIT_FAILURE;
	}


    m = LireImage(argv[1]);

    printf("\n%d  |  %d\n", m.width, m.height);

    m2 = elargir_image(m);

    printf("\n%d  |  %d\n", m2.width, m2.height);

    interpolerLignes(&m2);

    interpolerColonnes(&m2);

    EcrireImage(m2, argv[2]);

	


return EXIT_SUCCESS;
}

