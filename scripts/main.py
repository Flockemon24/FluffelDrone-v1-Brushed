import time
import paho.mqtt.client as mqtt
from pynput import keyboard
from dotenv import load_dotenv
import os

load_dotenv()

# --- Configuaration ---
MQTT_BROKER = os.getenv('MQTT_BROKER')
MQTT_PORT = 1883

# --- MQTT Client einrichten ---
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)

def connect_mqtt():
    try:
        client.connect(MQTT_BROKER, MQTT_PORT, 60)
        client.loop_start()
        print(f"Successfully conected with MQTT Broker ({MQTT_BROKER})!")
    except Exception as e:
        print(f"Connection failed: {e}")
        exit()

# --- Steuerbefehle senden ---
def send_commands():
    # Sendet die aktuellen Werte an die jeweiligen MQTT-Themen
    client.publish("drone/cmd/throttle", str(throttle))
    client.publish("drone/cmd/pitch", f"{pitch:.2f}")
    client.publish("drone/cmd/roll", f"{roll:.2f}")
    print(f"Send -> Gas: {throttle} | Pitch: {pitch:.1f} | Roll: {roll:.1f}", end="\r")

# --- Tastatur-Events verarbeiten ---
def on_press(key):
    global throttle, pitch, roll
    try:
        # Tasten für Gas (Höhe)
        if key.char == 'w':
            throttle = min(throttle + 15, 255)  # Gas erhöhen
        elif key.char == 's':
            throttle = max(throttle - 15, 0)    # Gas verringern
        
        # Tasten für Richtung (Pitch & Roll)
        elif key.char == 'i':
            pitch = 1.5   # Vorwärts neigen
        elif key.char == 'k':
            pitch = -1.5  # Rückwärts neigen
        elif key.char == 'j':
            roll = -1.5   # Nach links neigen
        elif key.char == 'l':
            roll = 1.5    # Nach rechts neigen
            
        # Not-Aus (Spacebar)
        elif key == keyboard.Key.space:
            throttle = 0
            pitch = 0.0
            roll = 0.0
            print("\n*** EMERGENCY-OFF ACTIVATED! ***")

        send_commands()

    except AttributeError:
        # Sonder- oder Funktionstasten ignorieren
        pass

def on_release(key):
    global pitch, roll
    try:
        # Wenn die Richtungstasten losgelassen werden, Drohne wieder gerade ausrichten
        if key.char in ['i', 'k']:
            pitch = 0.0
        elif key.char in ['j', 'l']:
            roll = 0.0
            
        send_commands()
    except AttributeError:
        pass

# --- Main-Loop ---
if __name__ == "__main__":
    print("Starte MQTT-Drohnen-Steuerung...")
    connect_mqtt()
    
    print("\n=== CONTROL-HINTS ===")
    print("W / S : Fly Faster / Slower")
    print("I / K : Fly Foreward / Backward")
    print("J / L : Fly Left / Right")
    print("LEERTASTE : EMERGENCY-OFF")
    print("PRESS CMD+C TO STOP")
    print("==========================\n")

    # Tastatur-Listener starten (läuft im Hintergrund)
    with keyboard.Listener(on_press=on_press, on_release=on_release) as listener:
        listener.join()