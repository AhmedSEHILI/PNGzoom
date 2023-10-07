<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Image Zoom using Cubic Spline Interpolation</title>
</head>
<body>

<h1>Image Zoom using Cubic Spline Interpolation</h1>

<h2>Introduction</h2>
<p>This project provides a simple C program for image processing that allows you to zoom in on an image using cubic spline interpolation. It takes an input image and produces an output image with enhanced zoomed-in details.</p>

<h2>How it Works</h2>
<p>The zooming process is achieved by performing the following steps:</p>
<ol>
    <li><strong>Initial Expansion:</strong> Two empty lines are added after each line of pixels, and two empty columns are added after each column in the pixels matrix of the image.</li>
    <li><strong>Cubic Spline Interpolation:</strong> The empty lines and columns created in the previous step are filled using cubic spline interpolation. Cubic spline interpolation is a mathematical technique that smoothly connects data points, resulting in a more detailed and visually pleasing zoomed image.</li>
</ol>

<h2>Implementation Details</h2>
<p>The code is written in C and uses the "lodepng" library for reading and writing images.</p>

<h2>Compilation and Execution</h2>
<p>To compile the code, use the following command:</p>
<pre><code>gcc -Wall zoom.c lodepng.c -o zoom</code></pre>

<p>To execute the program, run the following command:</p>
<pre><code>./zoom inputImage.png outputImage.png</code></pre>

<h2>Example</h2>
<p>Here's an example of how to use the program to zoom in on an input image named <code>inputImage.png</code> and save the zoomed result as <code>outputImage.png</code>:</p>
<pre><code>./zoom inputImage.png outputImage.png</code></pre>

<h2>Dependencies</h2>
<ul>
    <li>C Compiler (gcc)</li>
    <li>"lodepng" library for image reading and writing</li>
</ul>

<h2>Contributions</h2>
<p>Contributions and improvements to this project are welcome. Feel free to submit issues or pull requests.</p>

<h2>License</h2>
<p>This project is licensed under the MIT License - see the <a href="LICENSE">LICENSE</a> file for details.</p>

</body>
</html>
