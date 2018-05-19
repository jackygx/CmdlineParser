/*
 * Copyright (c) 2018 Guo Xiang
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __CMD_LINE_PARSER_HPP__
#define __CMD_LINE_PARSER_HPP__

#include <EasyCpp.hpp>

DEFINE_CLASS(CmdlineParser);

class CCmdlineParser
{
public:
	CCmdlineParser(int argc, char *argv[]);

	void AddKey(int8_t key, const char *desc);

	void Parse(void);

	bool IsKeySet(int8_t key);

	CStringPtr GetKeyArg(int8_t key, uint32_t i);

	CStringListPtr GetKeyArgs(int8_t key);

	template <class Fn>
	void GetKeyArgs(int8_t key, const Fn &fn);

	void PrintUsage(int retval = -1);

private:
	void CheckAndInitKeyParam(int8_t key);
	void ParseKey(int &i);

private:
	struct Param {
		int8_t bKey;		/* Is it a key. Set by AddKey */
		int8_t bSet;		/* Is the key set. Set by cmdline */
		int8_t idx;			/* Start index of the parameter */
		int8_t nParam;		/* Number of the parameter */
		const char *desc;
	};

#define MAX_PARAM_NUM 26
	int mArgc;
	char **mArgv;

	struct Param mParam[MAX_PARAM_NUM];
};

template <class Fn>
void CCmdlineParser::GetKeyArgs(int8_t key, const Fn &fn)
{
	int idx = key - 'a';

	CHECK_PARAM(idx < MAX_PARAM_NUM && idx >= 0,
				"Illegal key: ",
				HEX(key),
				"(", String::CHAR(key), ")");

	struct Param *param = &mParam[idx];

	for (int8_t i = param->idx; i < param->idx + param->nParam; ++i) {
		fn(mArgv[i]);
	}
}

#endif /* __CMD_LINE_PARSER_HPP__ */

