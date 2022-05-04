export module nException;
import vertex;

import <exception>;
import <string>;
import <vector>;
import <chrono>;

export namespace nechto
{
	struct exception : public std::exception
	{
		const vertexPtr Where;

		exception(const std::string pWhat = "unknown error", vertexPtr pWhere = nullVertexPtr)
			:std::exception(pWhat.c_str()), Where(pWhere){}

		exception(const char* const pWhat, vertexPtr pWhere = nullVertexPtr)
			:std::exception(pWhat, 0), Where(pWhere) {}
		virtual ~exception() {}

		virtual const char* what() const;
	};
}