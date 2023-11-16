# Bus_et_Reseau
Tp réalisé par Quentin Henriet et Clément Coutouillat

Ce projet consiste à faire communiquer différents capteurs et actionneur avec un STM32 de traiter les données et de les envoyer à une raspberry afin de les afficher sur une page web.

Dans un premier temps, nous avons réaliser la partie communication entre le STM32 et le capteur de pression BMP280. C'est capteur qui communique gràace à l'I2C. Après une première phase de lecture de documentation afin de connaitre l'adresse du composant et les différents registres pour récuperer les données, nous avons déider de creer deux fichiers, un fichier drv_bmp280.c qui comporte les fonctions à utiliser si on veux communiquer le BMP280. Un autre fichier drv_bmp280.h qui lui possède l'adresse des différents registres ainsi que les primitives de nos fonctions. 
