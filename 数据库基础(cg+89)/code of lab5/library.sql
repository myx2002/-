/*
    LIBRARY MANAGEMENT SYSTEM

    Source Server           : mysql
    Source Server Type      : MySQL
    Source Host             : localhost:3306

    Preparation conditions  :create a new database named "library"

    Particular attention:It may be necessary to rebuild the library database, 
                         and cascading deletion can only respond to a single delete statement, 
                         while cascading deletion of tables is not supported; If the library database 
                         is not rebuilt, it may cause auto_ Increment error increased.

    Last modification time  :2023.4.8
*/

SET NAMES utf8;

-- 
-- Table structure for admin
-- Format management, convenient language PHP matching
--
DROP TABLE IF EXISTS `admin`;
CREATE TABLE `admin`  (
  `admin_id` int NOT NULL,
  `password` varchar(15) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `admin_name` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  PRIMARY KEY (`admin_id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;
-- 
-- insert admin data
-- 
INSERT INTO `admin` VALUES (202301, '123456', 'admin');
INSERT INTO `admin` VALUES (202302, '', 'check');


-- 
-- Table structure for book
-- 
DROP TABLE IF EXISTS `book`;
create table `book` (
    `book_id` int NOT NULL auto_increment,
    `category` varchar(63) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
    `title` varchar(63) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
    `press` varchar(63) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
    `publish_year` int NOT NULL,
    `author` varchar(63) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
    `price` decimal(7, 2) NOT NULL DEFAULT 0.00,
    `stock` int NOT NULL  DEFAULT 0,
    PRIMARY KEY (`book_id`) USING BTREE,
    UNIQUE (`category`, `press`, `author`, `title`, `publish_year`)
)auto_increment=1 ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;
-- 
-- insert records into book
-- 
INSERT INTO `book` VALUES ('1','cs', 'good book', 'zju', 2004, 'myx', 10.02,10);
INSERT INTO `book`(`category`, `title`,`press`, `publish_year`, `author`, `price`, `stock`) VALUES ('cs', 'bad book', 'zju', 2011, 'cy', 11.32, 1);
INSERT INTO `book`(`category`, `title`,`press`, `publish_year`, `author`, `price`, `stock`) VALUES ('cs', 'book', 'xshs', 2007, 'cg', 5.65, 2);
INSERT INTO `book`(`category`, `title`,`press`, `publish_year`, `author`, `price`, `stock`) VALUES ('math', 'zju book', 'zju', 2013, 'cx', 11.32, 3);
INSERT INTO `book`(`category`, `title`,`press`, `publish_year`, `author`, `price`, `stock`) VALUES ('math', 'common book', 'pku', 2009, 'cx', 5.77, 6);
INSERT INTO `book`(`category`, `title`,`press`, `publish_year`, `author`, `price`, `stock`) VALUES ('english', 'english book', 'other', 2008, 'myx', 12.03, 8);
INSERT INTO `book`(`category`, `title`,`press`, `publish_year`, `author`, `price`, `stock`) VALUES ('chinese', 'my book', 'other', 2022, 'myx', 10.09, 6);
INSERT INTO `book`(`category`, `title`,`press`, `publish_year`, `author`, `price`, `stock`) VALUES ('science', 'your book', 'zju', 2014, 'myx', 7.02, 0);
INSERT INTO `book`(`category`, `title`,`press`, `publish_year`, `author`, `price`, `stock`) VALUES ('math', 'other book', 'xshs', 1994, 'cy', 16.02, 1);
INSERT INTO `book`(`category`, `title`,`press`, `publish_year`, `author`, `price`, `stock`) VALUES ('other', 'other book', 'xshs', 2003, 'other', 16.41, 3);
INSERT INTO `book`(`category`, `title`,`press`, `publish_year`, `author`, `price`, `stock`) VALUES ('children', 'zju book', 'zjg', 2005, 'other', 7.02, 2);
INSERT INTO `book`(`category`, `title`,`press`, `publish_year`, `author`, `price`, `stock`) VALUES ('english', 'zju book', 'yq', 2004, 'cg', 8.19, 0);
INSERT INTO `book`(`category`, `title`,`press`, `publish_year`, `author`, `price`, `stock`) VALUES ('cs', 'book hoop', 'yq', 2004, 'cg', 20.11, 2);
INSERT INTO `book`(`category`, `title`,`press`, `publish_year`, `author`, `price`, `stock`) VALUES ('other', 'book ox', 'yq', 2007, 'cg', 10.00, 2);


-- 
-- Table structure for card
-- 
DROP TABLE IF EXISTS `card`;
create table `card` (
    `card_id` int NOT NULL auto_increment,
    `name` varchar(63) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
    `department` varchar(63) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
    `type` char(1) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
    PRIMARY KEY (`card_id`) USING BTREE,
    UNIQUE (`department`, `type`, `name`),
    CHECK ( `type` in ('T', 'S') )
) auto_increment=1 ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;
-- 
-- insert records into card
-- 
INSERT INTO `card`(`name`,`department`,`type`)VALUES ('马宇啸','cs','S');
INSERT INTO `card`(`name`,`department`,`type`)VALUES ('梅岩枭','math','T');
INSERT INTO `card`(`name`,`department`,`type`)VALUES ('其他','other','S');


-- 
-- Table structure for borrow 
-- 
DROP TABLE IF EXISTS `borrow`;
create table `borrow` (
  `card_id` int NOT NULL,
  `book_id` int NOT NULL,
  `borrow_time` date NOT NULL,
  `return_time` date NULL DEFAULT NULL,
  `admin_id` int NOT NULL,
  Primary key (`card_id`, `book_id`, `borrow_time`) USING BTREE,
  FOREIGN KEY (`card_id`) references `card`(`card_id`) on delete cascade on update cascade,
  FOREIGN KEY  (`book_id`) references `book`(`book_id`) on delete cascade on update cascade
)ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;
-- 
-- insert records of borrow
-- 
INSERT INTO `borrow` VALUES ('1', '3', '2023-01-02', '2023-03-31', 202302);
INSERT INTO `borrow`(`card_id`,`book_id`,`borrow_time`,`admin_id`)VALUES ('3', '3', '2023-04-08', 202301);
INSERT INTO `borrow`(`card_id`,`book_id`,`borrow_time`,`admin_id`)VALUES ('3', '2', '2023-04-08', 202301);
INSERT INTO `borrow` VALUES ('2', '7', '2023-03-31', '2023-04-08', 202302);