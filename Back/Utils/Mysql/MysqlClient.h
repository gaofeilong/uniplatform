/// \file waMysqlClient.h
/// webapp::ysqlClient,webapp::MysqlData��ͷ�ļ�
/// MySQL���ݿ�C++�ӿ�

// �������:
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

/// \defgroup waMysqlClient waMysqlClient�������������ȫ�ֺ���

/// \ingroup waMysqlClient
/// \typedef MysqlDataRow 
/// MysqlData ���������� (map<string,string>)
typedef map<string,string> MysqlDataRow;

/// SQL����ַ�ת��
string EscapeSql(const string &str);

/// MySQL���ݼ���
class MysqlData {
        friend class MysqlClient;

public:
        /// MysqlData���캯��
        MysqlData():
        m_Rows(0), m_Cols(0), m_CurPos(0), m_Fetched(-1),
        m_MysqlRes(0), m_MysqlFields(0)
        {};

        /// MysqlData��������
        virtual ~MysqlData();
        
        /// ���MysqlData����
        bool FillData(MYSQL *mysql);

        /// ����ָ��λ�õ�MysqlData����
        /// \param row ��λ��
        /// \param col ��λ��
        /// \return �����ַ���
        inline string operator() (const size_t row, const size_t col) {
                return this->GetData(row, col);
        }
        /// ����ָ��λ�õ�MysqlData����
        string GetData(const size_t row, const size_t col);

        /// ����ָ���ֶε�MysqlData����
        /// \param row ��λ��
        /// \param field �ֶ���
        /// \return �����ַ���
        inline string operator() ( const size_t row, const string &field ) {
                return this->GetData(row, field);
        }
        /// ����ָ���ֶε�MysqlData����
        string GetData(const size_t row, const string &field);

        /// ����ָ��λ�õ�MysqlData������
        MysqlDataRow GetRow(const long row = -1);

        /// ����MysqlData��������
        inline size_t Rows() const {
                return m_Rows;
        }
        /// ����MysqlData��������
        inline size_t Cols() const {
                return m_Cols;
        }
        
        /// �����ֶ�λ��
        int FieldPos(const string &field);
        /// �����ֶ�����
        string FieldName(const size_t col) const;

private:
        /// ��ֹ���ÿ������캯��
        MysqlData(MysqlData &copy);
        /// ��ֹ���ÿ�����ֵ����
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

/// MySQL���ݿ�������
class MysqlClient {
public:
        /// MysqlĬ�Ϲ��캯��
        MysqlClient()
        :m_Connected(false)
        {}
        
        /// Mysql���캯��
        /// \param host MySQL����IP
        /// \param user MySQL�û���
        /// \param pwd �û�����
        /// \param database Ҫ�򿪵����ݿ�
        /// \param port ���ݿ�˿ڣ�Ĭ��Ϊ0
        /// \param socket UNIX_SOCKET��Ĭ��ΪNULL
        MysqlClient( const string &host, const string &user, const string &pwd, 
                const string &database, const int port = 0, const char* socket = NULL ) 
        {
                m_Connected = false;
                this->Connect(host, user, pwd, database, port, socket);
        }
        
        /// Mysql��������
        virtual ~MysqlClient() {
                this->Disconnect();
        }
        
        /// �������ݿ�
        bool Connect(const string &host, const string &user, const string &pwd, 
                const string &database, const int port = 0, const char* socket = NULL);
        /// �Ͽ����ݿ�����
        void Disconnect();
        /// �ж��Ƿ��������ݿ�
        bool IsConnected();
        
        /// ѡ�����ݿ�
        bool SelectDB(const string &database);

        /// ִ��SQL���,ȡ�ò�ѯ���
        bool Query(const string &sqlstr, MysqlData &records);
        /// ִ��SQL���
        bool Query(const string &sqlstr);
        
        /// ���ز�ѯ�����ָ��λ�õ��ַ���ֵ
        string QueryVal(const string &sqlstr, 
                const size_t row = 0, const size_t col = 0 );
        
        /// ���ز�ѯ�����ָ����
        MysqlDataRow QueryRow( const string &sqlstr, const size_t row = 0 );

        /// �ϴβ�ѯ������Ӱ��ļ�¼����
        size_t Affected();
        /// ȡ���ϴβ�ѯ��һ��AUTO_INCREMENT�����ɵ�ID
        size_t LastId();
        
        /// ȡ��Mysql������Ϣ
        /// \return ���ش�����Ϣ�ַ���
        inline string Error() {
                return string(mysql_error(&m_Mysql));
        }
        /// ȡ��Mysql������
        /// \return ���ش�����Ϣ���
        inline size_t ErrorNo() {
                return mysql_errno(&m_Mysql);
        }

        /// ȡ�ø�����Ϣ
        string Info();

        ////////////////////////////////////////////////////////////////////////////
private:
        
        /// ��ֹ���ÿ������캯��
        MysqlClient( MysqlClient &copy );
        /// ��ֹ���ÿ�����ֵ����
        MysqlClient& operator = (const MysqlClient& copy);

private:
        MYSQL m_Mysql;
        bool  m_Connected;
};

//} // namespace

#endif //_MYSQL_CLIENT_H_

