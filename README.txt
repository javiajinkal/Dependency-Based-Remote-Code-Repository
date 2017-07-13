//////////////////////////////////////////////////////////////////////////////////////
// Project4 OOD
// Jinkal Arvind Javia , SUID: 425325424
//////////////////////////////////////////////////////////////////////////////////////

--Automated test suite
For displaying automated testing of requirements the files are checkedin into Repository 
folder from the "Checkin" folder. 

The output displayed is not in sequential order because of threads

--Checkin using GUI
Please select the files to be checkedin, select the files on which it is dependent in the
Repository, select OPEN/CLOSE property, and then select the CHECKIN button to checkin file.
For each of the checked in file the metadata (xml) file will be generated in the folder.
Unique time data stamp is used for the generation of the folder. The checked in files are 
stored in the folder "Repository"

--Extract using GUI
Please select the file to be extracted from repository and select its extension and then 
click on EXTRACT button to extract the file. 

The files will be extracted in "ExtractedFiles" Folder

--To shut down the through SHUTDOWN BUTTONS in GUI
Please shut down the server first and then the client as according to my logic the client
will be sending a shut down message request to the server and hence server should shut down 
first and then the client. Otherwise it will result in crash
