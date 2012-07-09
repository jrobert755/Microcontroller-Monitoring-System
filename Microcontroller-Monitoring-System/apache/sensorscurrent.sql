-- phpMyAdmin SQL Dump
-- version 3.5.1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Jul 09, 2012 at 02:21 PM
-- Server version: 5.5.25
-- PHP Version: 5.4.4

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `sensorscurrent`
--

-- --------------------------------------------------------

--
-- Table structure for table `analogsensorconfiguration`
--

CREATE TABLE IF NOT EXISTS `analogsensorconfiguration` (
  `serial` varchar(16) NOT NULL,
  `nickname` varchar(64) NOT NULL,
  `pinstoread` int(1) NOT NULL,
  `readtime` int(1) NOT NULL,
  `pollcurve` tinyint(1) NOT NULL,
  `pollcurvestart` int(1) NOT NULL,
  `pollcurveend` int(1) NOT NULL,
  `pollcurvereadtime` int(1) NOT NULL,
  `pollcurveresistances` varchar(64) NOT NULL,
  `currentspot` int(1) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `analogsensorconfiguration`
--

INSERT INTO `analogsensorconfiguration` (`serial`, `nickname`, `pinstoread`, `readtime`, `pollcurve`, `pollcurvestart`, `pollcurveend`, `pollcurvereadtime`, `pollcurveresistances`, `currentspot`) VALUES
('abcdefghijklmnop', 'CIC', 31, 30, 1, 1341586564, 1341606564, 60, '1,2,3,4,5,6,5,4,3,2,1', 11);

-- --------------------------------------------------------

--
-- Table structure for table `analogsensorreadings`
--

CREATE TABLE IF NOT EXISTS `analogsensorreadings` (
  `serial` varchar(16) NOT NULL,
  `nickname` varchar(64) NOT NULL,
  `time` timestamp NULL DEFAULT NULL,
  `reading` double(30,10) NOT NULL,
  `pin` int(1) NOT NULL,
  `pollcurvereading` tinyint(1) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `analogsensorreadings`
--

INSERT INTO `analogsensorreadings` (`serial`, `nickname`, `time`, `reading`, `pin`, `pollcurvereading`) VALUES
('abcdefghijklmnop', 'CIC', '2012-07-06 16:06:05', 1.5000000000, 0, 0),
('abcdefghijklmnop', 'CIC', '2012-07-06 16:07:48', 1.6000000000, 0, 0),
('abcdefghijklmnop', 'CIC', '2012-07-06 16:47:40', 1.7000000000, 0, 0),
('abcdefghijklmnop', 'CIC', '2012-07-06 20:01:30', 1.8000000000, 0, 1),
('abcdefghijklmnop', 'CIC', '2012-07-06 20:01:30', 1.9000000000, 0, 1);

-- --------------------------------------------------------

--
-- Table structure for table `sensors`
--

CREATE TABLE IF NOT EXISTS `sensors` (
  `serial` varchar(16) NOT NULL,
  `nickname` varchar(64) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `sensors`
--

INSERT INTO `sensors` (`serial`, `nickname`) VALUES
('abcdefghijklmnop', 'CIC');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
