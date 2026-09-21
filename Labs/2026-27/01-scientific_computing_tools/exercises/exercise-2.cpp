#include <Eigen/Dense>
#include <cstdlib>
#include <iostream>

// from https://github.com/nothings/stb/tree/master
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace Eigen;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <image_path>" << std::endl;
    return 1;
  }

  const char *input_image_path = argv[1];

  // Load the image using stb_image
  int width, height, channels;
  // for greyscale images force to load only one channel
  unsigned char *image_data =
      stbi_load(input_image_path, &width, &height, &channels, 1);
  if (!image_data) {
    std::cerr << "Error: Could not load image " << input_image_path
              << std::endl;
    return 1;
  }
  std::cout << "Image loaded: " << width << "x" << height << " with "
            << channels << " channels in the file (forced to 1)." << std::endl;

  MatrixXd gray(height, width);

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      int index = (i * width + j); // 1 channel (grayscale)
      gray(i, j) = static_cast<double>(image_data[index]) / 255.0;
      ;
    }
  }

  MatrixXd darker = (gray.array() - (50.0 / 255.0)).cwiseMax(0.0).matrix();
  Matrix<unsigned char, Dynamic, Dynamic, RowMajor> modified_image(height,
                                                                   width);
  modified_image = darker.unaryExpr([](double val) -> unsigned char {
    return static_cast<unsigned char>(val * 255.0);
  });

  Matrix<unsigned char, Dynamic, Dynamic, RowMajor> rotated(width, height);
  rotated = modified_image.transpose();


  

  // Save the modified image using stb_image_write
  const std::string output_image_path = "output_modified.png";
  if (stbi_write_png(output_image_path.c_str(), height, width, 1,
                     modified_image.data(), height) == 0) {
    std::cerr << "Error: Could not save modified image" << std::endl;

    return 1;
  } 

  std::cout << "Modified image saved to " << output_image_path << std::endl;
  return 0;
}
