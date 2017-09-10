/*******************************************************************************
 *
 * @ File Name  : FilterType.h
 * @ Date       : 2012-10-17
 * @ Author     : gaofeilong <gaofeilonggmail@163.com>
 * @ Description: 
 * @ History    : 2012-10-17������
 *                2013-03-07: ���Ŀ¼���˽ṹ��
 *
 * ****************************************************************************/

#ifndef _FILTER_TYPE_H_
#define _FILTER_TYPE_H_

#include <set>
#include <string>
using std::set;
using std::string;

/* _NOFILTER����ָ������ˡ�_FILTER: ���ˣ�_DEMAND��ָ�� */
enum _FilterType { _NOFILTER, _FILTER, _DEMAND };

/* ����ǰ׺����׺ */
struct _FixFilterArg {
        enum _FilterType _Type;         /* ���ָ���� */
        set<string> _FixSet;            /* Ҫָ�������˵��ļ����ͼ��� */
};

/* ����ʱ�� */
struct _TimeFilterArg {
        enum   _FilterType _Type;       /* ֻ����1 */
        int    _TimeBoundaryNear;       /* ��������޸�ʱ�� */
        int    _TimeBoundaryFar;        /* ���������޸�ʱ�� */
        time_t _TimeNow;                /* ��ǰʱ�� */
};

/* ����Ŀ¼ */
struct _DirFilterArg {
        enum _FilterType _Type;         /* ���ָ���ˣ�ָ������� 0 */
        set<string> _DirSet;            /* Ҫָ�������˵��ļ�Ŀ¼���� */
};

/* �� Max �� Min �Ƿ�Ϊ 0 ����ʶ���ֹ��ˣ������С������ */
struct _SizeFilterArg {
        enum _FilterType _Type;         /* -1���ޣ�0: ���ˣ�1��ָ�� */
        size_t _Max;                    /* ָ���������ļ���С�����ֵ */
        size_t _Min;                    /* ָ���������ļ���С����Сֵ */
};

/* ָ�������������������ۺ� */
struct _FilterArg {
        struct _DirFilterArg _DirArg;
        struct _TimeFilterArg _TimeArg;
        struct _SizeFilterArg _SizeArg;
        struct _FixFilterArg _PrefixArg;
        struct _FixFilterArg _PostfixArg;
};

#endif  //_FILTER_TYPE_H_
