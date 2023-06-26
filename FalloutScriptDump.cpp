/**
 *
 * Copyright (c) 2005-2009 Anchorite (TeamX), <anchorite2001@yandex.ru>
 * Copyright (c) 2014-2015 Nirran, phobos2077
 * Copyright (c) 2015 alexeevdv <mail@alexeevdv.ru>
 * Distributed under the GNU GPL v3. For full terms see the file license.txt
 *
 */

// C++ standard includes
#include <iostream>
#include <fstream>

// int2ssl includes
#include "FalloutScript.h"
#include "Utility.h"

// Third party includes

void CFalloutScript::Dump()
{
    scriptData.outputStream << "============== Procedures table ==================" << std::endl
               << std::endl;
    m_ProcTable.Dump(scriptData);
    scriptData.outputStream << std::endl;
    scriptData.outputStream << std::endl;

    scriptData.outputStream << "============== Namespace ==================" << std::endl;
    m_Namespace.Dump(scriptData);
    scriptData.outputStream << std::endl;
    scriptData.outputStream << std::endl;

    scriptData.outputStream << "============== Stringspace ==================" << std::endl;
    m_Stringspace.Dump(scriptData);
    scriptData.outputStream << std::endl;
    scriptData.outputStream << std::endl;


    std::string strOutLine;
    uint16_t wOperator;
    uint32_t ulArgument = 0;

    scriptData.outputStream << "============== Global variables values ==================" << std::endl;

    if (m_GlobalVar.empty())
    {
        scriptData.outputStream << "Not found" << std::endl;
    }
    else
    {
        for(unsigned int i = 0; i < m_GlobalVar.size(); i++)
        {
            wOperator = m_GlobalVar[i].GetOperator();
            ulArgument = m_GlobalVar[i].GetArgument();

            switch(wOperator)
            {
                case COpcode::O_STRINGOP:
                case COpcode::O_INTOP:
                    scriptData.outputStream <<  format("%d: %s(0x%08x)   // %u (%d)",
                                    i,
                                    m_GlobalVar[i].GetAttributes().m_strMnemonic.c_str(),
                                    ulArgument,
                                    ulArgument,
                                    ulArgument) << std::endl;
                    break;

                case COpcode::O_FLOATOP:
                    scriptData.outputStream << format("%d: %s(0x%08x)   // %05f",
                                    i,
                                    m_GlobalVar[i].GetAttributes().m_strMnemonic.c_str(),
                                    ulArgument,
                                    *((float*)(&ulArgument))) << std::endl;
            }
        }
    }

    scriptData.outputStream << std::endl;
    scriptData.outputStream << std::endl;

    scriptData.outputStream << "============== Exported variables ==================" << std::endl;

    if (m_ExportedVarValue.empty())
    {
        scriptData.outputStream << "Not found" << std::endl;
    }
    else
    {
        for(uint32_t i = 0; i < m_ExportedVarValue.size(); i += 2)
        {
            wOperator = m_ExportedVarValue[i].GetOperator();
            ulArgument = m_ExportedVarValue[i].GetArgument();
            uint32_t ulNameArgument = m_ExportedVarValue[i + 1].GetArgument();

            switch(wOperator)
            {
                case COpcode::O_STRINGOP:
                    scriptData.outputStream << format("%s := \"%s\"",
                                    m_Namespace[ulNameArgument].c_str(),
                                    m_Stringspace[ulArgument].c_str()) << std::endl;
                    break;

                case COpcode::O_INTOP:
                    scriptData.outputStream << format("%s := %u (%d)",
                                    m_Namespace[ulNameArgument].c_str(),
                                    ulArgument,
                                    ulArgument) << std::endl;
                    break;

                case COpcode::O_FLOATOP:
                    scriptData.outputStream << format("%s := %05f",
                                    m_Namespace[ulNameArgument].c_str(),
                                    *((float*)(&ulArgument))) << std::endl;
            }
        }
    }

    scriptData.outputStream << std::endl;
    scriptData.outputStream << std::endl;

    scriptData.outputStream << "============== Procedures ==================" << std::endl;
    scriptData.outputStream << std::endl;

    for(uint32_t nIndexOfProc = 0; nIndexOfProc < m_ProcTable.GetSize(); nIndexOfProc++)
    {
        scriptData.outputStream << format("%d: %s (0x%08x)", nIndexOfProc,
                                               m_Namespace[m_ProcTable[nIndexOfProc].m_ulNameOffset].c_str(),
                                               m_ProcTable[nIndexOfProc].m_ulBodyOffset) << std::endl;
        scriptData.outputStream << "===============================" << std::endl;

        for(uint32_t i = 0; i < m_ProcBodies[nIndexOfProc].size(); i++)
        {
            wOperator = m_ProcBodies[nIndexOfProc][i].m_Opcode.GetOperator();
            ulArgument = m_ProcBodies[nIndexOfProc][i].m_Opcode.GetArgument();

            switch(wOperator)
            {
                case COpcode::O_STRINGOP:
                case COpcode::O_INTOP:
                    scriptData.outputStream << format("0x%08X: 0x%04X 0x%08x - %s(0x%08x)   // %u (%d)",
                                      m_ProcBodies[nIndexOfProc][i].m_ulOffset,
                                      wOperator,
                                      ulArgument,
                                      m_ProcBodies[nIndexOfProc][i].m_Opcode.GetAttributes().m_strMnemonic.c_str(),
                                      ulArgument,
                                      ulArgument,
                                      ulArgument) << std::endl;
                    break;

                case COpcode::O_FLOATOP:
                    scriptData.outputStream << format("0x%08X: 0x%04X 0x%08X - %s // %05f",
                                      m_ProcBodies[nIndexOfProc][i].m_ulOffset,
                                      wOperator,
                                      ulArgument,
                                      m_ProcBodies[nIndexOfProc][i].m_Opcode.GetAttributes().m_strMnemonic.c_str(),
                                      *((float*)(&ulArgument))) << std::endl;
                    break;

                default:
                    scriptData.outputStream << format("0x%08X: 0x%04X            - %s",
                                      m_ProcBodies[nIndexOfProc][i].m_ulOffset,
                                      wOperator, 
                                      m_ProcBodies[nIndexOfProc][i].m_Opcode.GetAttributes().m_strMnemonic.c_str()) << std::endl;
            }
        }

        scriptData.outputStream << std::endl;
    }
}
