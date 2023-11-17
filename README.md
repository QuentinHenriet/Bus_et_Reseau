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

Pour communiquer avec le moteur, nous allons utiliser le bus CAN. Pour cela, il faut avoir un baudrate précis qui ici est de 500kbit/s. Pour cela nous avons : 
- Prescaler de 12        
- Time Quanta in Bit Segment 1 = 3 times
- Time Quanta in Bit Segment 2 = 3 times
Nous allons ensuite initialiser la connexion avec la fonction "HAL_CAN_Start" et ensuite pour communiquer il faut utiliser HAL_CAN_AddTxMessage (&hcan1, &pHeader, aData, &TxMailbox) avec :
- pHeader.IDE = CAN_ID_STD;              Configure l'identifiant du message comme étant un identifiant standard 
- pHeader.StdId = 0x61;                  Définis le mode dans lequel le moteur sera utilisé (ici contrôle en angle)
- pHeader.RTR = CAN_RTR_DATA;            Indique que le message est un message de données           
- pHeader.DLC = 2;                       Configure la longueur des données du message à 2 octets.
- pHeader.TransmitGlobalTime = DISABLE;  Cela désactive la fonction de transmission du temps global

Et dans aData nous allons tout d'abord avoir l'angle auquel on veut que le moteur se positionne et ensuite, on aura une deuxième valeur qui indiquera si on veut être entre 0 et 180 ou entre 0 et -180.

Nous avons ensuite décidé de combiner le capteur BMP280 et le moteur afin qu'ils fonctionnent ensemble. Nous allons donc avoir un angle qui sera proportionnel à la température. 

Enfin, nous avons une partie réception de la commande de la raspberry. On va donc attendre de recevoir des données via l'UART4 et ensuite suivant le message reçu, on va transmettre les données voulues.
#############################################################################################################################################################################################################################
Partie Raspberrry


En lançant le serveur depuis hello.py on accès aux commandes du tp 4.3
soit post, get et delete , à l'adresse http://ip.du.pi.0/api/welcome/ 
ce qui permet de changer , visualiser ou supprimer la phrase d'acceuil
ainsi que GET PUT PATCH et DELETE à l'adresse http://ip.du.pi.0/api/welcome/x
ce qui permet de changer , visualiser ,ou ajouter le charactère à la position X dans la phrase.

En lançant le serveur depuis comm.py dont voici le code

import serial
import requests
import json

from flask import jsonify,redirect,abort,request
from flask import Flask
app = Flask(__name__)

commands = {
    "GET_T": "Temperature",
    "GET_P": "Pression",
    "SET_K=XXXX": "Set scale",
    "GET_K": "Scale",
    "GET_A": "Angle"
}

port = "/dev/ttyS0"
baudrate = 115200

ser = serial.Serial(port, baudrate, timeout=1)

command = input("Entrez la commande : ")

if command in commands:
    ser.write(command + "\n")
    response = ser.readline()
    ser.close()

    if command == "SET_K=XXXX":
        if response == "SET_K=OK":
            response = {"scale": XXXX}
        else:
            response = {"erreur": "Erreur"}
    else:
        response = {"valeur": response[:-1]}

    url = "http://localhost:5000/" + commands[command]
    if command == "SET_K=XXXX":
        url += "?k=" + str(XXXX)
    response = requests.post(url, data=json.dumps(response))

    if response.status_code == 200:
        print("Réponse postée avec succès")
    else:
        print("Erreur :", response.status_code)
else:
    print("Commande invalide")
@app.route('/temp/', methods=['GET'])
def get_all_temperatures():
    return jsonify(temperatures)

@app.route('/temp/<int:index>', methods=['GET'])
def get_temperature(index):
    if index < 0 or index >= len(temperatures):
        abort(404)

    return jsonify(temperatures[index])

@app.route('/pres/', methods=['GET'])
def get_all_pressures():
    return jsonify(pressures)

@app.route('/pres/<int:index>', methods=['GET'])
def get_pressure(index):
    if index < 0 or index >= len(pressures):
        abort(404)

    return jsonify(pressures[index])

@app.route('/scale/', methods=['GET'])
def get_scale():
    global scale

    if scale is None:
        abort(400)

    return jsonify(scale)

@app.route('/angle/', methods=['GET'])
def get_angle():
    global scale

    if scale is None:
        abort(400)

    temperatures_sum = sum(temperatures)

    if temperatures_sum == 0:
        abort(404)
temperatures = []
pressures = []
scale = None

@app.route('/temp/', methods=['GET'])
def get_all_temperatures():
    return jsonify(temperatures)

@app.route('/temp/<int:index>', methods=['GET'])
def get_temperature(index):
    if index < 0 or index >= len(temperatures):
        abort(404)

    return jsonify(temperatures[index])

@app.route('/pres/', methods=['GET'])
def get_all_pressures():
    return jsonify(pressures)

@app.route('/pres/<int:index>', methods=['GET'])
def get_pressure(index):
    if index < 0 or index >= len(pressures):
        abort(404)

    return jsonify(pressures[index])

@app.route('/scale/', methods=['GET'])
def get_scale():
    global scale

    if scale is None:
        abort(400)

    return jsonify(scale)

def get_angle():
    global scale

    if scale is None:
        abort(400)

    if temperatures == []:
        abort(404)

    last_temperature = temperatures[-1]
    angle = last_temperature * scale

    return jsonify(angle)
@app.route('/scale/<int:new_scale>', methods=['POST'])
def change_scale(new_scale):
    global scale
    scale = new_scale

    return jsonify(scale)

@app.route('/temp/', methods=['POST'])
def add_temperature():
    data = request.get_json()
    if not isinstance(data['temperature'], float):
        abort(400)

    temperatures.append(data['temperature'])

    return jsonify(temperatures)

@app.route('/pres/', methods=['POST'])
def add_pressure():
    data = request.get_json()
    if not isinstance(data['pressure'], float):
        abort(400)

    pressures.append(data['pressure'])
return jsonify(pressures)

@app.route('/temp/<int:index>', methods=['DELETE'])
def delete_temperature(index):
    if index < 0 or index >= len(temperatures):
        abort(404)

    temperatures.pop(index)

    return jsonify(temperatures)

@app.route('/pres/<int:index>', methods=['DELETE'])
def delete_pressure(index):
    if index < 0 or index >= len(pressures):
        abort(404)

    pressures.pop(index)

    return jsonify(pressures)



En tapant les commandes listées en haut permet d'envoyer une requête au stm32 et d'effectuer 
l'opération voulue
Post pour récupérer la valeur , et la mettre sur le serveur 
GET pour la visualiser 
DELETE pour supprimer une certaine valeur.
