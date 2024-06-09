import time
import requests
import serial
import serial.serialutil

# Bearer Tokenを設定
bearer_token = "anI3REVxVWljcXhsWFFKRDlQbUxobUlCVnpGRTE3SjFVNHdRMmFaOV9DUzhMOjE3MTczNzkxMzgzNDA6MToxOmF0OjE"

# エンドポイントのURL
url = "https://api.twitter.com/2/tweets"

# 投稿するテキスト
text = "あーーーー、絶起"

# リクエストヘッダーを設定
headers = {
    "Authorization": f"Bearer {bearer_token}",
    "Content-Type": "application/json"
}

# 投稿データを設定
data = {"text": text}

# Arduinoとのシリアル通信の設定
port = '/dev/tty.usbserial-AQ01L1W8'
baudrate = 9600
retry_count = 5
connected = False

for i in range(retry_count):
    try:
        ser = serial.Serial(port, baudrate)
        connected = True
        break
    except serial.serialutil.SerialException as e:
        print(f"Attempt {i + 1} failed: {e}")
        time.sleep(2)

if not connected:
    raise Exception("Could not open serial port after multiple attempts.")

while True:
    # Arduinoからのシリアルデータを読み取る
    arduino_data = ser.readline().decode().strip()
    if arduino_data == "sensor_on":
        # POSTリクエストを送信
        response = requests.post(url, headers=headers, json=data)
        # 応答を確認
        if response.status_code == 201:
            print("Tweet posted successfully!")
        else:
            print(f"Error: {response.status_code} - {response.text}")
    
    # 1秒待って再度監視する
    time.sleep(1)
