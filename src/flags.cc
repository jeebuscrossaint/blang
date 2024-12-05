#include "fifth.hh"
#include <cctype>

bool is_forth_file(const std::filesystem::path &path)
{
	std::string ext = path.extension().string();
	std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

	const std::vector<std::string> valid_extensions = {".fth", ".f", ".fs", ".4th"};

	return std::find(valid_extensions.begin(), valid_extensions.end(), ext) != valid_extensions.end();
}
