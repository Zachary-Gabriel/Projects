-- MySQL dump 10.17  Distrib 10.3.13-MariaDB, for Linux (x86_64)
--
-- Host: 127.0.0.1    Database: enrolment
-- ------------------------------------------------------
-- Server version	10.3.13-MariaDB

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Current Database: `enrolment`
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `enrolment` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci */;

USE `enrolment`;

--
-- Table structure for table `Enrolments`
--

DROP TABLE IF EXISTS `Enrolments`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Enrolments` (
  `student_id` varchar(8) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `subject_code` varchar(63) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `mark` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Enrolments`
--

LOCK TABLES `Enrolments` WRITE;
/*!40000 ALTER TABLE `Enrolments` DISABLE KEYS */;
INSERT INTO `Enrolments` VALUES ('a1111115','COMP SCI 1102',62),('a1111115','COMP SCI 2000',80),('a1111112','COMP SCI 1102',55),('a1111112','COMP SCI 2207',80),('a1111113','PHIL 2039',65),('a1111113','COMP SCI 1102',46),('a1111116','COMP SCI 2207',67),('a1111116','COMP SCI 2000',49),('a1111118','COMP SCI 2000',48),('a1111117','COMP SCI 2207',75),('a1111120','COMP SCI 2207',81),('a1111118','ENTREP 1011',73),('a1111117','ENG 1002',91),('a1111120','ELEC ENG 1102',38),('a1111119','ENTREP 1011',57),('a1111119','BIOCHEM 2500',91),('a1111119','COMP SCI 2000',70);
/*!40000 ALTER TABLE `Enrolments` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Students`
--

DROP TABLE IF EXISTS `Students`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Students` (
  `student_id` varchar(8) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `name` varchar(63) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `program` varchar(63) COLLATE utf8mb4_unicode_ci DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Students`
--

LOCK TABLES `Students` WRITE;
/*!40000 ALTER TABLE `Students` DISABLE KEYS */;
INSERT INTO `Students` VALUES ('a1111112','Jane Brown','BE(Hons)(Soft)'),('a1111113','Bob Smith','BCompSc'),('a1111115','Dot Broadbent','BCompSc'),('a1111116','Raleigh Lovell','BCompSc'),('a1111117','Roxane Palmer','BMaCompSc'),('a1111118','Jin Chu','BE(Hons)(Soft)'),('a1111119','Ming Lam','BMaCompSc'),('a1111120','Tai Jiang','BE(Hons)(Soft)');
/*!40000 ALTER TABLE `Students` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Subjects`
--

DROP TABLE IF EXISTS `Subjects`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Subjects` (
  `subject_code` varchar(63) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `subject` varchar(63) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `faculty` varchar(63) COLLATE utf8mb4_unicode_ci DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Subjects`
--

LOCK TABLES `Subjects` WRITE;
/*!40000 ALTER TABLE `Subjects` DISABLE KEYS */;
INSERT INTO `Subjects` VALUES ('ENG 1002','Programming (MATLAB and C)','ECMS'),('ELEC ENG 1102','Digital Electronics','ECMS'),('COMP SCI 1102','Object Oriented Programming','ECMS'),('COMP SCI 2207','Web and Database Computing','ECMS'),('COMP SCI 2000','Computer Systems','ECMS'),('PHIL 2039','Philosophy of Mind','Arts'),('ENTREP 1011','Entrepreneurship Foundations and Mindset','Business'),('BIOCHEM 2500','Biochemistry II: Molecular and Cell Biology','Science');
/*!40000 ALTER TABLE `Subjects` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2019-04-15  0:18:23
