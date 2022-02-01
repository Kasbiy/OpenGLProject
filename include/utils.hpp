#ifndef UTILS_HPP
#define UTILS_HPP

#include "Visualizer.hpp"

BEGIN_VISUALIZER_NAMESPACE

bool LoadFile(const std::string& fileName, std::string& result);
void DisplayLastWinAPIError();

END_VISUALIZER_NAMESPACE

#endif // !UTILS_HPP
