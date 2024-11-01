template<typename T> 
std::string tostring(const T& value)
{
	std::stringstream stream;
	stream << value;
	return stream.str();
}