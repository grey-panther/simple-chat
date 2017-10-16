#include "ISocketAddress.hpp"


ISocketAddress::operator std::string() const
{
	return ip() + ':' + std::to_string(port());
}


std::ostream& operator<<(std::ostream& out, const ISocketAddress& address)
{
	return out << static_cast<std::string>(address);
}
