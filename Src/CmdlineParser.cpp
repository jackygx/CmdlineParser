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

#include <EasyCpp.hpp>
#include <CmdlineParser.hpp>

#ifdef DEBUG_CMDLINE_PARSER
#define DEBUG(fmt, ...) printf(fmt"\n", __VA_ARGS__)
#else
#define DEBUG(...)
#endif

CCmdlineParser::CCmdlineParser(int argc, char *argv[]) :
	mArgc(argc),
	mArgv(argv)
{
	for (int i = 0; i < MAX_PARAM_NUM; ++i) {
		mParam[i].bKey = 0;
	}
}

void CCmdlineParser::AddKey(int8_t key, const char *desc)
{
	CHECK_PARAM(NULL != desc, "Invalid description");

	int idx = key - 'a';
	CHECK_PARAM(idx < MAX_PARAM_NUM && idx >= 0,
				"Illegal key: ",
				HEX(key),
				"(", String::CHAR(key), ")");

	struct Param *param = &mParam[idx];
	param->bKey = 1;
	param->bSet = 0;
	param->idx = 0;
	param->nParam = 0;
	param->desc = desc;
}

void CCmdlineParser::CheckAndInitKeyParam(int8_t key)
{
	DEBUG("Check and init key: '%c'", key + 'a');

	if (key < 0 || key >= MAX_PARAM_NUM) {
		PrintUsage(-1);
	}

	struct Param *param = &mParam[key];

	/* Check if the key is valid */
	if (0 == param->bKey) {
		PrintUsage(-1);
	}

	param->bSet = 1;
	param->nParam = 0;
}

void CCmdlineParser::ParseKey(int &i)
{
	const char *argv = mArgv[i];

	/* Must have at least one key */
	int8_t key = argv[1] - 'a';
	CheckAndInitKeyParam(key);

	/* Single key mode, may have addition parameters */
	if (0 == argv[2]) {
		struct Param *param = &mParam[key];
		param->idx = ++i;

		for (; i < mArgc; ++i) {
			argv = mArgv[i];
			if ('-' == argv[0]) {
				ParseKey(i);
				return;
			} else {
				DEBUG("Add arg(%d) for key: '%c'", i, key + 'a');
				++param->nParam;
			}
		}

		/* Multi key mode, cannot have addition parameters */
	} else {
		uint32_t j = 2;

		do {
			CheckAndInitKeyParam(argv[j++] - 'a');
		} while (0 != argv[j]);
	}
}

void CCmdlineParser::Parse(void)
{
	for (int i = 1; i < mArgc; ++i) {
		const char *argv = mArgv[i];

		/* Key must start with '-' */
		if ('-' != argv[0]) {
			PrintUsage(-1);
		}

		ParseKey(i);
	}
}

bool CCmdlineParser::IsKeySet(int8_t key)
{
	int idx = key - 'a';
	CHECK_PARAM(idx < MAX_PARAM_NUM && idx >= 0,
				"Illegal key: ",
				HEX(key),
				"(", String::CHAR(key), ")");

	return 0 != mParam[idx].bSet;
}

CStringListPtr CCmdlineParser::GetKeyArgs(int8_t key)
{
	CStringListPtr list;

	int idx = key - 'a';
	CHECK_PARAM(idx < MAX_PARAM_NUM && idx >= 0,
				"Illegal key: ",
				HEX(key),
				"(", String::CHAR(key), ")");

	struct Param *param = &mParam[idx];
	DEBUG("Dumping param for key: %d", key);
	DEBUG("  bKey: %d", param->bKey);
	DEBUG("  bSet: %d", param->bSet);
	DEBUG("  idx: %d", param->idx);
	DEBUG("  nParam: %d", param->nParam);

	for (int8_t i = param->idx; i < param->idx + param->nParam; ++i) {
		list->PushBack(mArgv[i]);
	}

	return list;
}

CStringPtr CCmdlineParser::GetKeyArg(int8_t key, uint32_t i)
{
	int idx = key - 'a';
	CHECK_PARAM(idx < MAX_PARAM_NUM && idx >= 0,
				"Illegal key: ",
				HEX(key),
				"(", String::CHAR(key), ")");

	struct Param *param = &mParam[idx];
	DEBUG("Dumping param for key: %d", key);
	DEBUG("  bKey: %d", param->bKey);
	DEBUG("  bSet: %d", param->bSet);
	DEBUG("  idx: %d", param->idx);
	DEBUG("  nParam: %d", param->nParam);

	return (i < (uint32_t)param->nParam) ?
		mArgv[param->idx + i] : nullptr;
}

void CCmdlineParser::PrintUsage(int retval)
{
	printf("Usage: %s [OPTION]...\n", mArgv[0]);
	for (uint32_t i = 0; i < MAX_PARAM_NUM; ++i) {
		struct Param *param = &mParam[i];
		if (0 != param->bKey) {
			printf("\t-%c\t\t\t%s\n", i + 'a', mParam[i].desc);
		}
	}

	exit(retval);
}

