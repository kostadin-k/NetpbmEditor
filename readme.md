# Netpbm Editor  
A program for editing Netpbm images. Supports only the P1, P2 and P3 formats.

## Operations
+ ### Converting
    Supports converting between formats on saving. (Dithering is suggested before converting to P1.)
+ ### Cropping
    Cropping by selecting any 2 points. Bigger coordinates than the image are restricted to the image dimantions. Minimal selection is 1 pixel (achieved with equal coordinates).
+ ### Resizring
    Resizing using the nearest neighbour alorithm. Image dimentions can be resized by inputing new ones or by scaling the entire image with a percentage.  
    More description in article: https://tannerhelland.com/2012/12/28/dithering-eleven-algorithms-source-code.html
+ ### Dithering
    11 different dithering algorithms.   
    More description in article: https://tannerhelland.com/2012/12/28/dithering-eleven-algorithms-source-code.html
    
    1. Floyd-Steinberg Dithering
    2. Fake Floyd-Steinberg Dithering
    3. Jarvis, Judice, and Ninke Dithering
    4. Stucki Dithering
    5. Atkinson Dithering
    6. Burkes Dithering
    7. Sierra Dithering (5x3)
    8. Sierra Dithering (5x2)
    9. Sierra Dithering (3x2)
    10. Ordered dithering (4x4)
    11. Ordered dithering (8x8)

## Commands
+ new \<height> \<width> \<color (#hex)>
+ open \<path>
+ close
+ save [path]
+ info
+ crop \<point1-x> \<point1-y> \<point2-x> \<point2-y>
+ resize \<percentage (decimal%)> OR resize \<height> \<width>
+ dither \<algorithm (number)>
+ exit