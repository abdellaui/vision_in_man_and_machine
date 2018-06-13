
/*
  Abdullah Sahin
  Tolga Akkiraz
*/

#include <pragma.h>
#include <iostream>

int pragma::main(int argc, char *argv[])
{
  // settings
  const pragma::string imageName = "lena.jpg";
  const pragma::string relPathToImage = "../images/" + imageName;

  /*
    ####  TASK 1  ####
  */

  pragma::Image::ColorImagePointer image;
  image->readJPEG(relPathToImage);

  // manipulate some pixels, let make a grid
  for (int x = 0; x < image->xResolution(); x++)
  {
    for (int y = 0; y < image->yResolution(); y++)
    {
      if (x % 0xF == 0 || y % 0xF == 0)
        image->pixel(x, y, pragma::Color(255, 255, 255));
    }
  }
  // show result
  pragma::Graphics::WindowPointer window(imageName, 0, 0);
  window->image(image, image->xResolution(), image->yResolution());


  /*
    ####  TASK 2  ####
  */


  pragma::Image::ByteImagePointer imageBW;
  imageBW->readJPEG(relPathToImage);
  // its automaticly black and white, so just need to rotate 180° and maybe save local to a rel. path
  imageBW->rotate180();
  imageBW->writeJPEG("bw_"+imageName);


  // ctrl + c to close
  std::getchar();
  return 0;
}
