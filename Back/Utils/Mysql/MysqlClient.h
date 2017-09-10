/// \file waMysqlClient.h
/// webapp::ysqlClient,webapp::MysqlData类头文件
/// MySQL数据库C++接口

// 编译参数:
// (CC) -I /usr/local/include/mysql/ -L /usr/local/lib/mysql -lmysqlclient -lm

#ifndef _MYSQL_CLIENT_H_
#define _MYSQL_CLIENT_H_

#include <string>
#include <vector>
#include <map>
#include <mysql/mysql.h>

//using namespace std;
using std::map;
using std::vector;
using std::string;

/// Web Application Library namaspace
//namespace gw {

/// \defgroup waMysqlClient waMysqlClient相关数据类型与全局函数

/// \ingroup waMysqlClient
/// \typedef MysqlDataRow 
/// MysqlData 数据行类型 (map<string,string>)
typedef map<string,string> MysqlDataRow;

/// SQL语句字符转义
string EscapeSql(const string &str);

/// MySQL数据集类
class MysqlData {
        friend class MysqlClient;

public:
        /// MysqlData构造函数
        MysqlData():
        m_Rows(0), m_Cols(0), m_CurPos(0), m_Fetched(-1),
        m_MysqlRes(0), m_MysqlFields(0)
        {};

        /// MysqlData析构函数
        virtual ~MysqlData();
        
        /// 填充MysqlData数据
        bool FillData(MYSQL *mysql);

        /// 返回指定位置的MysqlData数据
        /// \param row 行位置
        /// \param col 列位置
        /// \return 数据字符串
        inline string operator() (const size_t row, const size_t col) {
                return this->GetData(row, col);
        }
        /// 返回指定位置的MysqlData数据
        string GetData(const size_t row, const size_t col);

        /// 返回指定字段的MysqlData数据
        /// \param row 行位置
        /// \param field 字段名
        /// \return 数据字符串
        inline string operator() ( const size_t row, const string &field ) {
                return this->GetData(row, field);
        }
        /// 返回指定字段的MysqlData数据
        string GetData(const size_t row, const string &field);

        /// 返回指定位置的MysqlData数据行
        MysqlDataRow GetRow(const long row = -1);

        /// 返回MysqlData数据行数
        inline size_t Rows() const {
                return m_Rows;
        }
        /// 返回MysqlData数据列数
        inline size_t Cols() const {
                return m_Cols;
        }
        
        /// 返回字段位置
        int FieldPos(const string &field);
        /// 返回字段名称
        string FieldName(const size_t col) const;

private:
        /// 禁止调用拷贝构造函数
        MysqlData(MysqlData &copy);
        /// 禁止调用拷贝赋值操作
        MysqlData& operator = (const MysqlData& copy);

protected:
        
        size_t m_Rows;
        size_t m_Cols;
        size_t m_CurPos;
        long   m_Fetched;

        MYSQL_RES       *m_MysqlRes;
        MYSQL_ROW       m_MysqlRow;
        MYSQL_FIELD     *m_MysqlFields;
        map<string,int> m_FieldPos;

};

/// MySQL数据库连接类
class MysqlClient {
public:
        /// Mysql默认构造函数
        MysqlClient()
        :m_Connected(false)
        {}
        
        /// Mysql构造函数
        /// \param host MySQL主机IP
        /// \param user MySQL用户名
        /// \param pwd 用户口令
        /// \param database 要打开的数据库
        /// \param port 数据库端口，默认为0
        /// \param socket UNIX_SOCKET，默认为NULL
        MysqlClient( const string &host, const string &user, const string &pwd, 
                const string &database, const int port = 0, const char* socket = NULL ) 
        {
                m_Connected = false;
                this->Connect(host, user, pwd, database, port, socket);
        }
        
        /// Mysql析构函数
        virtual ~MysqlClient() {
                this->Disconnect();
        }
        
        /// 连接数据库
        bool Connect(const string &host, const string &user, const string &pwd, 
                const string &database, const int port = 0, const char* socket = NULL);
        /// 断开数据库连接
        void Disconnect();
        /// 判断是否连接数据库
        bool IsConnected();
        
        /// 选择数据库
        bool SelectDB(const string &database);

        /// 执行SQL语句,取得查询结果
        bool Query(const string &sqlstr, MysqlData &records);
        /// 执行SQL语句
        bool Query(const string &sqlstr);
        
        /// 返回查询结果中指定位置的字符串值
        string QueryVal(const string &sqlstr, 
                const size_t row = 0, const size_t col = 0 );
        
        /// 返回查询结果中指定行
        MysqlDataRow QueryRow( const string &sqlstr, const size_t row = 0 );

        /// 上次查询动作所影响的记录条数
        size_t Affected();
        /// 取得上次查询的一个AUTO_INCREMENT列生成的ID
        size_t LastId();
        
        /// 取得Mysql错误信息
        /// \return 返回错误信息字符串
        inline string Error() {
                return string(mysql_error(&m_Mysql));
        }
        /// 取得Mysql错误编号
        /// \return 返回错误信息编号
        inline size_t ErrorNo() {
                return mysql_errno(&m_Mysql);
        }

        /// 取得更新信息
        string Info();

        ////////////////////////////////////////////////////////////////////////////
private:
        
        /// 禁止调用拷贝构造函数
        MysqlClient( MysqlClient &copy );
        /// 禁止调用拷贝赋值操作
        MysqlClient& operator = (const MysqlClient& copy);

private:
        MYSQL m_Mysql;
        bool  m_Connected;
};

//} // namespace

#endif //_MYSQL_CLIENT_H_

