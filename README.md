# Bus_et_Reseau
Tp réalisé par Quentin Henriet et Clément Coutouillat

Ce projet consiste à faire communiquer différents capteurs et actionneur avec un STM32 de traiter les données et de les envoyer à une raspberry afin de les afficher sur une page web.

Dans un premier temps, nous avons réalisé la partie communication entre le STM32 et le capteur de pression BMP280. C'est capteur qui communique grâce à l'I2C. Après une première phase de lecture de documentation afin de connaitre l'adresse du composant et les différents registres pour récupérer les données, nous avons décidé de creer deux fichiers, un fichier drv_bmp280.c qui comporte les fonctions à utiliser si on veut communiquer le BMP280, un autre fichier drv_bmp280.h qui lui possède l'adresse des différents registres ainsi que les primitives de nos fonctions. 


La configuration du BMP280 : 
- L'adresse du capteur est 0x77 et il est sur les ports PB7(SDA) et PB6(SCL) de l'I2C1
- Nous utilisons la configuration suivante: mode normal, Pressure oversampling x16, Temperature oversampling x2
- Pour la compensation des valeurs de températures et de pressions le code est celui donné dans la datasheet
Premièrement, dans le main il faut appeler la fonction BMP280_check afin de vérifier s'il y a bien un périphérique relié à l'i2C. Ensuite, nous avons la fonction "bmp280_init" qui va permettre d'initialiser le capteur avec le mode et la calibration que l'on veut. Après, il aut appeler la fonction "BMP280_get_temperature" ou "BMP280_get_pressure" suivant si on veut la temperature ou la pression. Et enfin, nous avons les fonctions permettant la calibration des données, ces fonctions servent à avoir des valeurs qui soient compréhensibles et interprétables.


La configuration du moteur (utilisation du bus CAN) :
Pour communiquer avec le moteur, nous allons utiliser le bus CAN. Pour cela, il faut avoir un baudrate précis
