//Antony Saputra D00197615
//Conor Byrne D00185566

template<typename T>

std::string toString(const T& value)
{
	std::stringstream stream;
	stream << value;
	return stream.str();
}
