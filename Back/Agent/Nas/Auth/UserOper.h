#ifndef __USER_OPER_H__
#define __USER_OPER_H__

#include <string>
#include <vector>
#include "NetUser.h"
//#include "../DealService/ServiceAccess.h"

#define OLD_USERS  "/etc/nas/Conf/oldUsers.txt"
#define OLD_USERS_TMP  "/etc/nas/Conf/oldUsers_tmp.txt"
#define OLD_GROUPS "/etc/nas/Conf/oldGroups.txt"
#define OLD_GROUPS_TMP "/etc/nas/Conf/oldGroups_tmp.txt"
class CUserOper
{
public:
        enum GROUP_TYPE 
        {
                GROUP_START = 0, 
                GROUP_LOCAL, 
                GROUP_LDAP, 
                GROUP_AD,
                GROUP_NIS,
                GROUP_END, 
        };

        //用户信息
        struct UserInfo
        {
                unsigned int uid; //用户ID
                std::string  user; //用户名
                std::string  userInfo; //用户全称
                std::string  pwd; //密码
                unsigned int gid; //私有组ID
                std::string  gname; //groupname
                //AD需要设置webdav和rsync的密码，否则无法支持共享
                //AD需要设置smb、webdav和rsync的密码，否则无法支持共享
                int          issetpwd; 
                UserInfo() {
                        uid = 0;
                        gid = 0;
                        issetpwd = 0;
                }
        };

        //组信息
        struct GroupInfo
        {
                unsigned int gid;                        //组id
                std::string group;              //组名称
                std::string groupInfo;          //组描述
                std::vector<std::string> innerUser;          //组成员
                GroupInfo() {
                        gid = 0;
                }
        };
        
        struct UserGroupInfo
        {
                UserInfo userInfo;
                GroupInfo groupInfo;
        };

typedef std::vector<UserInfo> UserInfoList;
typedef std::vector<GroupInfo> GroupInfoList;

public:
        CUserOper();
        virtual ~CUserOper();

        static CUserOper* Create(int type);

        static bool GetGroupList(std::vector<std::string>& list);
        static bool GetGroupAndUserList(GroupInfoList& glist);
        
        //用户操作
        //获取用户列表
        virtual bool GetUserList(UserInfoList& list) = 0;
        //添加新用户
        virtual bool AddUser(const UserInfo& info) = 0;
        //删除用户,isReal是否真实删除
        virtual bool DelUser (const UserInfo& info, bool isReal = false) = 0;
        //修改用户密码
        virtual bool SetUserPwd(const UserInfo& info) = 0;
        //确认用户名及密码
        virtual bool Check(const UserInfo& info) = 0;

        //组操作
        //获取组列表
        virtual bool GetGroupList(GroupInfoList& list) = 0;
        //获取指定组的用户列表
        virtual bool GetUserList(UserInfoList& list, unsigned int gId) = 0;
        //添加新组
        virtual bool AddGroup(const GroupInfo& info) = 0;
        //删除组
        virtual bool DelGroup(const GroupInfo& info) = 0;

        //用户&组混合操作
        //向组内添加用户,该操作不会真实的删除用户
        //virtual bool AddUserToGroup(const std::string& user, const std::string& group, unsigned int uid, unsigned int gid) = 0;
        //组内删除指定用户        
        //virtual bool DelUserFromGroup(const std::string& user, const std::string& group, unsigned int uid, unsigned int gid) = 0;
        
        //批量添加用户
        virtual bool  AddMultipleUsers(UserInfoList& list) = 0;
        
        //批量删除用户
        virtual bool  DelMultipleUsers(UserInfoList& list) = 0; 
        
        //批量添加用户组
        virtual bool  AddMultipleGroups(GroupInfoList & list) = 0;      
        
        //批量删除用户组
        virtual bool  DelMultipleGroups(GroupInfoList& list) = 0;
private:
        int m_type;
public:
        
        //获取本地保存的AD/NIS历史用户
        static void GetLastUsers(UserInfoList& list);
        
        //更新本地的AD/NIS用户
        static void UpdatetoLastUsers(UserInfoList& list);
        
        //更新本地的AD/NIS用户，标志是否设置webdav和rsync密码
        static void UpdateUser(UserInfo& user);
        
        //获取本地保存的AD/NIS历史用户组
        static void GetLastGroups(GroupInfoList& list);
        
        //更新本地的AD/NIS用户组
        static void UpdatetoLastGroups(GroupInfoList& list);
        
        //清空组或者用户配额
        static void EmptyQuota(uint id, int type);
};

#endif /* __USER_OPER_H__ */
