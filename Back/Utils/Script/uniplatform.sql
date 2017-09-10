-- phpMyAdmin SQL Dump
-- version 3.0.1.1
-- http://www.phpmyadmin.net
--
-- Host: 127.0.0.1
-- Generation Time: Apr 27, 2013 at 11:17 AM
-- Server version: 5.1.47
-- PHP Version: 5.3.2

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";


-- --------------------------------------------------------


--
-- Database: `uniplatform`
--

DROP DATABASE IF EXISTS `uniplatform`;
CREATE DATABASE IF NOT EXISTS `uniplatform` CHARACTER SET utf8;
USE `uniplatform`;
GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' IDENTIFIED BY '' WITH GRANT OPTION;
flush privileges;


-- --------------------------------------------------------

--
-- Table structure for table `email_config`
--

CREATE TABLE IF NOT EXISTS `email_config` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `state` tinyint(1),
  `server` char(16),
  `send` char(32),
  `send_passwd` char(16),
  `verify` char(8),
  `recv` text DEFAULT NULL,
  `level` char(32) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;


--
-- Dumping data for table `email_config`
--

-- INSERT INTO `email_config` (`id`, `state`, `server`, `send`, `send_passwd`, `verify`, `recv`, `level`) VALUES
-- (1, 1, '192.168.0.1', 'flgao@scidata.com', '111111', 'common', 'gfl1988101@gmail.com;', 'info;');
-- INSERT INTO `email_config` (`id`, `state`, `server`, `send`, `send_passwd`, `verify`, `recv`, `level`) VALUES
-- (2, 1, '192.168.0.1', 'flgao@scidata.com', '111111', 'common', 'gfl1988102@gmail.com;bwang1@scidata.com;', 'warning;');
-- INSERT INTO `email_config` (`id`, `state`, `server`, `send`, `send_passwd`, `verify`, `recv`, `level`) VALUES
-- (4, 1, '192.168.0.1', 'flgao@scidata.com', '111111', 'common', 'gfl1988104@gmail.com;', 'error;');
-- INSERT INTO `email_config` (`id`, `state`, `server`, `send`, `send_passwd`, `verify`, `recv`, `level`) VALUES
-- (3, 1, '192.168.0.1', 'flgao@scidata.com', '111111', 'common', 'gfl1988103@gmail.com;bwang2@scidata.com;', 'info;warning;');
-- INSERT INTO `email_config` (`id`, `state`, `server`, `send`, `send_passwd`, `verify`, `recv`, `level`) VALUES
-- (5, 1, '192.168.0.1', 'flgao@scidata.com', '111111', 'common', 'gfl1988105@gmail.com;bwang3@scidata.com;', 'info;error;');
-- INSERT INTO `email_config` (`id`, `state`, `server`, `send`, `send_passwd`, `verify`, `recv`, `level`) VALUES
-- (6, 1, '192.168.0.1', 'flgao@scidata.com', '111111', 'common', 'gfl1988106@gmail.com;bwang4@scidata.com;', 'warning;error;');
-- INSERT INTO `email_config` (`id`, `state`, `server`, `send`, `send_passwd`, `verify`, `recv`, `level`) VALUES
-- (7, 1, '192.168.0.1', 'flgao@scidata.com', '111111', 'common', 'gfl1988107@gmail.com;bwang5@scidata.com;', 'info;warning;error;');


-- --------------------------------------------------------

--
-- Table structure for table `user`
--

CREATE TABLE IF NOT EXISTS `user` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `username` char(16) NOT NULL,
  `password` char(64) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;

INSERT INTO `uniplatform`.`user` (`id`, `username`, `password`)
VALUES (NULL, 'admin', '96e79218965eb72c92a549dd5a330112'); 


--
-- Dumping data for table `user`
--


-- --------------------------------------------------------

--
-- Table structure for table `archive`
--

CREATE TABLE IF NOT EXISTS `archive` (
  `id` int(10) unsigned NOT NULL,
  `name` char(32) NOT NULL,
  `start_time` char(6) NOT NULL,
  `duration` int(10) NOT NULL,
  `start_interval` int(10) NOT NULL,
  `modify_time` int(10) NOT NULL,
  `is_check` smallint NOT NULL,
  `is_delete_source` smallint NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


--
-- Dumping data for table `archive`
--

-- INSERT INTO `uniplatform`.`archive` (`name`, `node`, `conf`)
-- VALUES ('archive1', '192.168.1.91', '/etc/scigw/archive/config/3/archive.conf'); 
-- INSERT INTO `uniplatform`.`archive` (`name`, `node`, `conf`)
-- VALUES ('archive2', '192.168.1.91', '/etc/scigw/archive/config/2/GWconfig'); 

-- --------------------------------------------------------


--
-- Table structure for table `archive`
--

CREATE TABLE IF NOT EXISTS `archive_filter` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `archive_id` int(10) unsigned NOT NULL,
  `prefix_type` smallint NOT NULL,
  `prefix` text default NULL,
  `postfix_type` smallint NOT NULL,
  `postfix` text default NULL,
  `dir_type` smallint NOT NULL,
  `dir` text default NULL,
  `size_type` smallint NOT NULL,
  `min_size` bigint(10) default NULL,
  `max_size` bigint(10) default NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;


--
-- Dumping data for table `archive_filter`
--


-- --------------------------------------------------------


--
-- Table structure for table `node_archive_state`
--

CREATE TABLE IF NOT EXISTS `node_archive_state` (
  `archive_id`    int(10) unsigned NOT NULL,
  `data_node_id`  int(10) NOT NULL,
  `counter`       int(4) NOT NULL,
  `archive_state` char(10) NOT NULL,
  `is_error`      smallint DEFAULT 0,
  PRIMARY KEY (`archive_id`, `data_node_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


--
-- Dumping data for table `node_archive_state`
--


-- --------------------------------------------------------

--
-- Table structure for table `archive_path_pair`
--

CREATE TABLE IF NOT EXISTS `archive_path_pair` (
  `config_id` int(10) unsigned NOT NULL,
  `src_path` text NOT NULL,
  `dest_path` text NOT NULL,
  `backup_path` text NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8; 


--
-- Dumping data for table `archive`
--
-- INSERT INTO `uniplatform`.`archive_path_pair` (`config_id`, `src_path`, `dest_path`, `backup_path`) 
-- VALUES (3, '/mnt/archive/src1', '/mnt/archive/dest1', '/mnt/archive/src.bak');

-- --------------------------------------------------------

--
-- Table structure for table `archive_log`
--

CREATE TABLE IF NOT EXISTS `archive_log` (
  `id` int(16) unsigned NOT NULL AUTO_INCREMENT,
  `ip` char(16) NOT NULL,
  `config_id` int(10) unsigned NOT NULL DEFAULT 0,
  `arvname` char(32) NOT NULL,
  `arvtime` datetime NOT NULL,
  `mtime` int(64) NOT NULL,
  `src` text NOT NULL,
  `dest` text NOT NULL,
  `size` int(64) NOT NULL,
  `state` char(8),
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8; 


--
-- Dumping data for table `archive`
--


-- --------------------------------------------------------

--
-- Table structure for table `ddfsmp`
--

CREATE TABLE IF NOT EXISTS `ddfsmp` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `mp` text NOT NULL,
  `config` text NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8; 


--
-- Dumping data for table `ddfsmp`
--
-- INSERT INTO `uniplatform`.`ddfsmp` (`mp`, `config`) VALUES ('/mnt/ddfsmp1', '/test/ddfs/config')


-- --------------------------------------------------------

--
-- Table structure for table `adfsmp`
--

CREATE TABLE IF NOT EXISTS `adfsmp` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `firstdatapath` text NOT NULL,
  `otherdatapath` text NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8; 


--
-- Dumping data for table `adfsmp`
--
-- INSERT INTO `uniplatform`.`adfsmp` (`firstdatapath`, `otherdatapath`) VALUES ('/mnt/adfs/db1', '/mnt/adfs/db2:/mnt/adfs/db3')


-- --------------------------------------------------------

--
-- Table structure for table `nfsserver`
--

CREATE TABLE IF NOT EXISTS `nfsserver` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` char(32) NOT NULL,
  `path` text NOT NULL,
  `mp` text NOT NULL,
  `parameter` text NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8; 


--
-- Dumping data for table `nfsserver`
--
-- INSERT INTO `uniplatform`.`nfsserver` (`name`, `path`, `mp`, `parameter`) 
-- VALUES ('share1', '/mnt/nfsserver1', '/mnt/nfsclient1', 'rw,sync,no_root_squash,fsid=0');
-- INSERT INTO `uniplatform`.`nfsserver` (`name`, `path`, `mp`, `parameter`) 
-- VALUES ('share2', '/mnt/nfsserver2', '/mnt/nfsclient2', 'rw,sync,root_squash,fsid=0');


-- --------------------------------------------------------

--
-- Table structure for table `nfsclient`
--

CREATE TABLE IF NOT EXISTS `nfsclient` (
  `serverid` int(10) NOT NULL,
  `ip` char(16) NOT NULL,
  `password` char(64) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8; 


--
-- Dumping data for table `nfsclient`
--
-- INSERT INTO `uniplatform`.`nfsclient` (`serverid`, `ip`, `password`) 
-- VALUES (1, '192.168.2.92', 'gfl');
-- INSERT INTO `uniplatform`.`nfsclient` (`serverid`, `ip`, `password`) 
-- VALUES (1, '192.168.2.93', 'gfl');
-- INSERT INTO `uniplatform`.`nfsclient` (`serverid`, `ip`, `password`) 
-- VALUES (1, '192.168.2.94', 'gfl');
-- INSERT INTO `uniplatform`.`nfsclient` (`serverid`, `ip`, `password`) 
-- VALUES (1, '192.168.2.95', 'gfl');
-- INSERT INTO `uniplatform`.`nfsclient` (`serverid`, `ip`, `password`) 
-- VALUES (2, '192.168.2.96', '111111');

-- --------------------------------------------------------

--
-- Table structure for table `weblog`
--

CREATE TABLE IF NOT EXISTS `weblog` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `time` datetime NOT NULL,
  `module` char(32) NOT NULL,
  `description` text NOT NULL,
  `loglevel` char(8) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8; 


--
-- Dumping data for table `weblog`
--
-- INSERT INTO `uniplatform`.`weblog` (`time`, `module`, `description`, loglevel)
-- VALUES ('2013-5-21 14:46:52', '系统设置', '查看系统信息成功', 'info'); 
-- INSERT INTO `uniplatform`.`weblog` (`time`, `module`, `description`, loglevel)
-- VALUES ('2013-5-21 14:49:52', '存储管理', '查看卷组信息失败, server error', 'error'); 
-- 


-- --------------------------------------------------------

--
-- Table structure for table `fbs_connection_test`
--

CREATE TABLE IF NOT EXISTS `fbs_connection_test` (
  `ip` char(16) NOT NULL,
  `user` char(16) NOT NULL,
  `password` char(64) NOT NULL,
  `type` char(8) NOT NULL,
  `indexid` int NOT NULL DEFAULT -1,
  `state` char(32) NOT NULL,
  PRIMARY KEY (`ip`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8; 


--
-- Dumping data for table `fbs_connection_test`
--


-- --------------------------------------------------------

--
-- Table structure for table `fbs_install`
--

CREATE TABLE IF NOT EXISTS `fbs_install` (
  `ip` char(16) NOT NULL,
  `user` char(16) NOT NULL,
  `password` char(64) NOT NULL,
  `type` char(8) NOT NULL,
  `indexid` int NOT NULL DEFAULT -1,
  `progress` int NOT NULL,
  `description` char(32) NOT NULL,
  `state` char(32) NOT NULL,
  PRIMARY KEY (`ip`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8; 


--
-- Dumping data for table `fbs_install`
--


-- --------------------------------------------------------

--
-- Table structure for table `fbs_index_node`
--

CREATE TABLE IF NOT EXISTS `fbs_index_node` (
  `id` int(16) unsigned NOT NULL AUTO_INCREMENT,
  `ip` char(16) NOT NULL,
  `user` char(32) NOT NULL,
  `password` char(32) NOT NULL,
  `idxrootpath` text NOT NULL DEFAULT "",
  `capacity1` bigint NOT NULL DEFAULT 0,
  `used1` bigint NOT NULL DEFAULT 0,
  `left1` bigint NOT NULL DEFAULT 0,
  `capacity2` bigint NOT NULL DEFAULT 0,
  `used2` bigint NOT NULL DEFAULT 0,
  `left2` bigint NOT NULL DEFAULT 0,
  `total` bigint NOT NULL DEFAULT 0,
  `real` bigint NOT NULL DEFAULT 0,
  `ratio` float NOT NULL DEFAULT 0,
  `updatetime` datetime NOT NULL DEFAULT '1970-01-01 08:00:00',
  `state` char(8) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8; 


--
-- Dumping data for table `fbs_index_node`
--


-- --------------------------------------------------------

--
-- Table structure for table `fbs_data_node`
--

CREATE TABLE IF NOT EXISTS `fbs_data_node` (
  `id` int(16) unsigned NOT NULL AUTO_INCREMENT,
  `indexid` int(16) NOT NULL,
  `ip` char(16) NOT NULL,
  `user` char(32) NOT NULL,
  `password` char(32) NOT NULL,
  `idxpath` text NOT NULL DEFAULT "",
  `mtpath` text NOT NULL DEFAULT "",
  `dbpath` text NOT NULL DEFAULT "",
  `isphoto` char(4) NOT NULL DEFAULT "",
  `capacity` bigint NOT NULL DEFAULT 0,
  `used` bigint NOT NULL DEFAULT 0,
  `left` bigint NOT NULL DEFAULT 0,
  `total` bigint NOT NULL DEFAULT 0,
  `real` bigint NOT NULL DEFAULT 0,
  `ratio` float NOT NULL DEFAULT 0,
  `updatetime` datetime NOT NULL DEFAULT '1970-01-01 08:00:00',
  `state` char(8) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8; 


--
-- Dumping data for table `fbs_data_node`
--


-- --------------------------------------------------------

--
-- Table structure for table `fbs_remove`
--

CREATE TABLE IF NOT EXISTS `fbs_remove` (
  `ip` char(16) NOT NULL,
  `user` char(16) NOT NULL,
  `password` char(64) NOT NULL,
  `type` char(8) NOT NULL,
  `indexid` int NOT NULL DEFAULT -1,
  `progress` int NOT NULL,
  `description` char(32) NOT NULL,
  `state` char(32) NOT NULL,
  PRIMARY KEY (`ip`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8; 


--
-- Dumping data for table `fbs_remove`
--


-- --------------------------------------------------------
