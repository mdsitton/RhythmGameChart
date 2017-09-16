// Copyright (c) 2015-2017 Matthew Sitton <matthewsitton@gmail.com>
// See LICENSE in the RhythmGameChart root for license information.

#pragma once

#include <string>
#include <fstream>

namespace RGCCPP::RGC
{

	class RgcWriter
	{
	public:
		RgcWriter(std::string filePath);
	private:
		std::string m_filePath;
		std::ofstream m_rgcFile;
		void read_file();
	};
}