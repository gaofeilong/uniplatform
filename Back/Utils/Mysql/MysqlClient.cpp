/// \file waMysqlClient.cpp
/// webapp::MysqlClient,webapp::MysqlData��ʵ���ļ�

// �������:
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
/// SQL����ַ�ת��
/// \param Ҫת����SQL�ַ���
/// \return ת������ַ���
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

/// MysqlData��������
MysqlData::~MysqlData()
{
        if (m_MysqlRes != NULL ) {
                mysql_free_result(m_MysqlRes);
        }
        m_MysqlRes    = 0;
        m_MysqlFields = 0;
}

/// ����ָ��λ�õ�MysqlData����
/// \param row ������λ��,Ĭ��Ϊ0
/// \param col ������λ��,Ĭ��Ϊ0
/// \return ��������,�������򷵻ؿ��ַ���
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

/// ����ָ���ֶε�MysqlData����
/// \param row ��λ��
/// \param field �ֶ���
/// \return �����ַ���,�����ڷ��ؿ��ַ���
string MysqlData::GetData(const size_t row, const string &field)
{
        int col = this->FieldPos(field);
        if ( col != -1 ) {
                return this->GetData(row, col);
        } else {
                return string( "" );
        }
}

/// ����ָ��λ�õ�MysqlData������
/// \param row ������λ��,Ĭ��Ϊ��ǰ��¼λ��,
/// ��ǰ��¼λ����first(),prior(),next(),last(),find()��������,Ĭ��Ϊ0
/// \return ����ֵ����ΪMysqlDataRow,��map<string,string>
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

/// ���MysqlData����
/// \param mysql MYSQL*����
/// \retval true �ɹ�
/// \retval false ʧ��
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

/// �����ֶ�λ��
/// \param field �ֶ���
/// \return �����ݽ���д��ڸ��ֶ��򷵻��ֶ�λ��,���򷵻�-1
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

/// �����ֶ�����
/// \param col �ֶ�λ��
/// \return �����ݽ���д��ڸ��ֶ��򷵻��ֶ�����,���򷵻ؿ��ַ���
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

/// �������ݿ�
/// \param host MySQL����IP
/// \param user MySQL�û���
/// \param pwd �û�����
/// \param database Ҫ�򿪵����ݿ�
/// \param port ���ݿ�˿ڣ�Ĭ��Ϊ0
/// \param socket UNIX_SOCKET��Ĭ��ΪNULL
/// \retval true �ɹ�
/// \retval false ʧ��
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

/// �Ͽ����ݿ�����
void MysqlClient::Disconnect()
{
        if(m_Connected) {
                mysql_close(&m_Mysql);
                m_Connected = false;
        }
}

/// �ж��Ƿ��������ݿ�
/// \retval true ����
/// \retval false �Ͽ�
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

/// ѡ�����ݿ�
/// \param database ���ݿ���
/// \retval true �ɹ�
/// \retval false ʧ��
bool MysqlClient::SelectDB(const string &database)
{
        if ( m_Connected && mysql_select_db(&m_Mysql,database.c_str())==0 ) {
                return true;
        } else {
                return false;
        }
}

/// ִ��SQL���,ȡ�ò�ѯ���
/// \param sqlstr Ҫִ�е�SQL���
/// \param records �������ݽ����MysqlData����
/// \retval true �ɹ�
/// \retval false ʧ��
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

/// ִ��SQL���
/// \param sqlstr Ҫִ�е�SQL���
/// \retval true �ɹ�
/// \retval false ʧ��
bool MysqlClient::Query( const string &sqlstr )
{
        LOG_INF("execute sql: %s", sqlstr.c_str());
        if ( m_Connected && mysql_real_query(&m_Mysql,sqlstr.c_str(),sqlstr.length())==0 ) {
                return true;
        } else {
                return false;
        }
}

/// ���ز�ѯ�����ָ��λ�õ��ַ���ֵ
/// \param sqlstr SQL��ѯ�ַ���
/// \param row ������λ��,Ĭ��Ϊ0
/// \param col ������λ��,Ĭ��Ϊ0
/// \return ��ѯ�ɹ������ַ���,���򷵻ؿ��ַ���
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

/// ���ز�ѯ�����ָ����
/// \param sqlstr SQL��ѯ�ַ���
/// \param row ������λ��,Ĭ��Ϊ0
/// \return ����ֵ����ΪMysqlDataRow,��map<string,string>
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

/// �ϴβ�ѯ������Ӱ��ļ�¼����
/// \return ���ؼ�¼����,����size_t
size_t MysqlClient::Affected()
{
        if (m_Connected) {
                return mysql_affected_rows( &m_Mysql );
        } else {
                return 0;
        }
}

/// ȡ���ϴβ�ѯ��һ��AUTO_INCREMENT�����ɵ�ID
/// һ��Mysql��ֻ����һ��AUTO_INCREMENT��,�ұ���Ϊ����
/// \return �������ɵ�ID
size_t MysqlClient::LastId()
{
        if (m_Connected) {
                return mysql_insert_id(&m_Mysql);
        } else {
                return 0;
        }
}

/// ȡ�ø�����Ϣ
/// \return ���ظ�����Ϣ
string MysqlClient::Info()
{
        if (m_Connected) {
                return mysql_info(&m_Mysql)?string(mysql_info(&m_Mysql)):string("");
        } else {
                return string("");
        }
}

//} // namespace

