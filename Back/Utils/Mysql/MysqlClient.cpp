/// \file waMysqlClient.cpp
/// webapp::MysqlClient,webapp::MysqlData类实现文件

// 编译参数:
// (CC) -I /usr/local/include/mysql/ -L /usr/local/lib/mysql -lmysqlclient -lm

#include <cstring>
#include "MysqlClient.h"
#include "Utils/Log/Log.h"

//using namespace std;
using std::string;

/// Web Application Library namaspace
//namespace gw {

/// \ingroup waMysqlClient
/// \fn string escape_sql( const string &str )
/// SQL语句字符转义
/// \param 要转换的SQL字符串
/// \return 转义过的字符串
string EscapeSql(const string &str)
{
        char *p = new char[str.length()*2+1];
        mysql_escape_string( p, str.c_str(), str.length() );
        string s = p;
        delete[] p;
        return s;
}

////////////////////////////////////////////////////////////////////////////
// MysqlData

/// MysqlData析构函数
MysqlData::~MysqlData()
{
        if (m_MysqlRes != NULL ) {
                mysql_free_result(m_MysqlRes);
        }
        m_MysqlRes    = 0;
        m_MysqlFields = 0;
}

/// 返回指定位置的MysqlData数据
/// \param row 数据行位置,默认为0
/// \param col 数据列位置,默认为0
/// \return 返回数据,不存在则返回空字符串
string MysqlData::GetData(const size_t row, const size_t col)
{
        if(m_MysqlRes!=NULL && row<m_Rows && col<m_Cols) {
                if (static_cast<long>(row) != m_Fetched) {
                        if ( row != m_CurPos+1 ) {
                                mysql_data_seek(m_MysqlRes, row);
                        }
                        m_MysqlRow = mysql_fetch_row(m_MysqlRes);
                        m_Fetched  = static_cast<long>(row); 
                }
                
                if (m_MysqlRow!=NULL && m_MysqlRow[col]!=NULL) {
                        m_CurPos = row; // log current cursor
                        return string(m_MysqlRow[col]);
                }
        }
        return string("");
}

/// 返回指定字段的MysqlData数据
/// \param row 行位置
/// \param field 字段名
/// \return 数据字符串,不存在返回空字符串
string MysqlData::GetData(const size_t row, const string &field)
{
        int col = this->FieldPos(field);
        if ( col != -1 ) {
                return this->GetData(row, col);
        } else {
                return string( "" );
        }
}

/// 返回指定位置的MysqlData数据行
/// \param row 数据行位置,默认为当前纪录位置,
/// 当前纪录位置由first(),prior(),next(),last(),find()函数操纵,默认为0
/// \return 返回值类型为MysqlDataRow,即map<string,string>
MysqlDataRow MysqlData::GetRow(const long row)
{
        MysqlDataRow datarow;
        string field;
        size_t rowpos;
        
        if ( row < 0 ) {
                rowpos = m_CurPos;
        } else {
                rowpos = row;
        }
                
        if( m_MysqlRes!=NULL && rowpos<m_Rows ) {
                if ( rowpos != m_CurPos ) {
                        if ( rowpos != m_CurPos+1 ) {
                                mysql_data_seek(m_MysqlRes, rowpos);
                        }
                        m_MysqlRow = mysql_fetch_row(m_MysqlRes);
                }
                
                if (m_MysqlRow != NULL) {
                        m_CurPos = rowpos; // log current cursor
                        for (size_t i=0; i<m_Cols; ++i) {
                                field = this->FieldName(i);
                                if (field!="" && m_MysqlRow[i]!=NULL) {
                                        datarow.insert(MysqlDataRow::value_type(field,m_MysqlRow[i]));
                                }
                        }
                }
        }
        return datarow;
}

/// 填充MysqlData数据
/// \param mysql MYSQL*参数
/// \retval true 成功
/// \retval false 失败
bool MysqlData::FillData(MYSQL *mysql)
{
        if (mysql == NULL) {
                return false;
        }
        
        // clean                
        if (m_MysqlRes != NULL) {
                mysql_free_result(m_MysqlRes);
        }
        m_MysqlRes = 0;
        m_CurPos   = 0; // return to first position
        m_FieldPos.clear(); // clean field pos cache

        // fill data
        m_MysqlRes = mysql_store_result(mysql);
        if (m_MysqlRes != NULL) {
                m_Rows = mysql_num_rows(m_MysqlRes);
                m_Cols = mysql_num_fields(m_MysqlRes);
                m_MysqlFields = mysql_fetch_fields(m_MysqlRes);
                
                // init first data
                mysql_data_seek(m_MysqlRes, 0);
                m_MysqlRow = mysql_fetch_row(m_MysqlRes);               

                return true;
        }
        return false;
}

/// 返回字段位置
/// \param field 字段名
/// \return 若数据结果中存在该字段则返回字段位置,否则返回-1
int MysqlData::FieldPos(const string &field)
{
        if (m_MysqlFields==0 || field=="") {
                return -1;
        }
        
        // check cache
        if (m_FieldPos.find(field) != m_FieldPos.end()) {
                return m_FieldPos[field];
        }

        for(size_t i=0; i<m_Cols; ++i) {
                if (strcmp(field.c_str(), m_MysqlFields[i].name) == 0) {
                        m_FieldPos[field] = i;
                        return i;
                }
        }
        m_FieldPos[field] = -1;
        return -1;
}

/// 返回字段名称
/// \param col 字段位置
/// \return 若数据结果中存在该字段则返回字段名称,否则返回空字符串
string MysqlData::FieldName(size_t col) const
{
        if (m_MysqlFields!=0 && col<=m_Cols) {
                return string(m_MysqlFields[col].name);
        } else {
                return string("");
        }
}

////////////////////////////////////////////////////////////////////////////
// MysqlClient

/// 连接数据库
/// \param host MySQL主机IP
/// \param user MySQL用户名
/// \param pwd 用户口令
/// \param database 要打开的数据库
/// \param port 数据库端口，默认为0
/// \param socket UNIX_SOCKET，默认为NULL
/// \retval true 成功
/// \retval false 失败
bool MysqlClient::Connect( const string &host, const string &user, const string &pwd, 
        const string &database, const int port, const char* socket )
{
        this->Disconnect();
        
        if (mysql_init(&m_Mysql)) {
                if ( mysql_real_connect( &m_Mysql, host.c_str(), user.c_str(),
                                                  pwd.c_str(), database.c_str(),
                                                  port, socket, 0 ) ) //CLIENT_COMPRESS ) )
                        m_Connected = true;
        }
        return m_Connected;
}

/// 断开数据库连接
void MysqlClient::Disconnect()
{
        if(m_Connected) {
                mysql_close(&m_Mysql);
                m_Connected = false;
        }
}

/// 判断是否连接数据库
/// \retval true 连接
/// \retval false 断开
bool MysqlClient::IsConnected()
{
        if (m_Connected) {
                if (mysql_ping(&m_Mysql) == 0) {
                        m_Connected = true;
                } else {
                        m_Connected = false;
                }
        }
        
        return m_Connected;
}

/// 选择数据库
/// \param database 数据库名
/// \retval true 成功
/// \retval false 失败
bool MysqlClient::SelectDB(const string &database)
{
        if ( m_Connected && mysql_select_db(&m_Mysql,database.c_str())==0 ) {
                return true;
        } else {
                return false;
        }
}

/// 执行SQL语句,取得查询结果
/// \param sqlstr 要执行的SQL语句
/// \param records 保存数据结果的MysqlData对象
/// \retval true 成功
/// \retval false 失败
bool MysqlClient::Query( const string &sqlstr, MysqlData &records )
{
        LOG_INF("execute sql: %s", sqlstr.c_str());
        if ( m_Connected && mysql_real_query(&m_Mysql,sqlstr.c_str(),sqlstr.length())==0 ) {
                if( records.FillData(&m_Mysql) ) {
                        return true;
                }
        }
        return false;
}

/// 执行SQL语句
/// \param sqlstr 要执行的SQL语句
/// \retval true 成功
/// \retval false 失败
bool MysqlClient::Query( const string &sqlstr )
{
        LOG_INF("execute sql: %s", sqlstr.c_str());
        if ( m_Connected && mysql_real_query(&m_Mysql,sqlstr.c_str(),sqlstr.length())==0 ) {
                return true;
        } else {
                return false;
        }
}

/// 返回查询结果中指定位置的字符串值
/// \param sqlstr SQL查询字符串
/// \param row 数据行位置,默认为0
/// \param col 数据列位置,默认为0
/// \return 查询成功返回字符串,否则返回空字符串
string MysqlClient::QueryVal(const string &sqlstr, const size_t row, 
        const size_t col)
{
        MysqlData data;
        if ( this->Query(sqlstr,data) ) {
                if ( data.Rows()>row && data.Cols()>col ) {
                        return data(row,col);
                }
        }
        return string( "" );
}

/// 返回查询结果中指定行
/// \param sqlstr SQL查询字符串
/// \param row 数据行位置,默认为0
/// \return 返回值类型为MysqlDataRow,即map<string,string>
MysqlDataRow MysqlClient::QueryRow(const string &sqlstr, const size_t row)
{
    MysqlData data;
    MysqlDataRow datarow;
    if ( this->Query(sqlstr,data) ) {
        if ( row < data.Rows() ) {
            datarow = data.GetRow(row);
        }
    }
    return datarow;
}

/// 上次查询动作所影响的记录条数
/// \return 返回记录条数,类型size_t
size_t MysqlClient::Affected()
{
        if (m_Connected) {
                return mysql_affected_rows( &m_Mysql );
        } else {
                return 0;
        }
}

/// 取得上次查询的一个AUTO_INCREMENT列生成的ID
/// 一个Mysql表只能有一个AUTO_INCREMENT列,且必须为索引
/// \return 返回生成的ID
size_t MysqlClient::LastId()
{
        if (m_Connected) {
                return mysql_insert_id(&m_Mysql);
        } else {
                return 0;
        }
}

/// 取得更新信息
/// \return 返回更新信息
string MysqlClient::Info()
{
        if (m_Connected) {
                return mysql_info(&m_Mysql)?string(mysql_info(&m_Mysql)):string("");
        } else {
                return string("");
        }
}

//} // namespace

