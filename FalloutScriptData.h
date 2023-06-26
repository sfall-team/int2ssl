/**
 *
 * Copyright (c) 2005-2009 Anchorite (TeamX), <anchorite2001@yandex.ru>
 * Copyright (c) 2014-2015 Nirran, phobos2077
 * Copyright (c) 2015 alexeevdv <mail@alexeevdv.ru>
 * Distributed under the GNU GPL v3. For full terms see the file license.txt
 *
 */

#pragma once

// C++ standard includes
#include <fstream>

// int2ssl includes

// Third party includes

struct CFalloutScriptData
{
    std::ifstream inputStream;
    std::ofstream outputStream;
};
