#pragma once

// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� CTPTRADESTRATEGY_EXPORT
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// CTP_TRADE_STRATEGY_API ������Ϊ�� DLL ���룬���� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef CTP_TRADE_STRATEGY_EXPORT
#define CTP_TRADE_STRATEGY_API __declspec(dllexport)
#else
#define CTP_TRADE_STRATEGY_API __declspec(dllimport)
#endif