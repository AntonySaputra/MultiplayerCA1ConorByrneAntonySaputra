//Antony Saputra D00197615

template<typename T>

std::string toString(const T& value)
{
	std::stringstream stream;
	stream << value;
	return stream.str();
}
