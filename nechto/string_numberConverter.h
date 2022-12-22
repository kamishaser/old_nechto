#pragma once
#include "typeDeclarations.h"
#include <string>

namespace nechto
{
	class snConverter
	{
	public:
		static ustr to_ustr(i64 number)
		{
			return toUstr(std::to_string(number));
		}
		static ustr to_ustr(f64 number)
		{
			return toUstr(std::to_string(number));
		}
		static ustr to_ustr(ushort number)
		{
			return toUstr(std::to_string(number));
		}
		static ustr to_ustr(ui32 number)
		{
			return toUstr(std::to_string(number));
		}
		static ustr to_ustr(int number)
		{
			return toUstr(std::to_string(number));
		}
		static ustr to_ustr(long number)
		{
			return toUstr(std::to_string(number));
		}
		static ustr to_ustr(short number)
		{
			return toUstr(std::to_string(number));
		}

		static i64 ustr_to_i64(ustr us)
		{
			return std::stoll(toString(us));
		}
		static f64 ustr_to_f64(ustr us)
		{
			return std::stod(toString(us));
		}
	private:
		static char to_char(char16_t ch)
		{
			if (ch == u'-')
				return '-';
			if (ch == u'.')
				return '.';
			return static_cast<char>(ch - u'0') + '0';
		}
		static char16_t to_16char(char ch)
		{
			if (ch == '-')
				return u'-';
			if (ch == '.')
				return u'.';
			return static_cast<char16_t>(ch - '0') + u'0';
		}
		static std::string toString(ustr us)
		{
			std::string s;
			s.resize(us.size());
			for (int i = 0; i < us.size(); ++i)
				s[i] = to_char(us[i]);
			return s;
		}
		static ustr toUstr(std::string s)
		{
			ustr us;
			us.resize(s.size());
			for (int i = 0; i < us.size(); ++i)
				us[i] = to_16char(s[i]);
			return us;
		}
	};
}