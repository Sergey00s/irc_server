import socket
import time
# Sunucu IP adresi ve port numarası
server_ip = "127.0.0.1"  # Sunucu IP'sini buraya yazın
server_port = 6667    # Sunucu port numarasını buraya yazın

# Bir soket oluştur
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

client_socket.connect((server_ip, server_port))
"""
hello
günaydın
naber
iyiyim
sen
ben

"""
while 1:
    # Sunucuya bağlan
    print("Sunucuya bağlandı.")

    msg = "GÜnaydın"
    # Sunucudan veri al
    msg = f"BOT 123456789 ANO {msg} \n"   
    client_socket.send(msg.encode("utf-8"))
    print("Sunucuya mesaj gönderildi.")
    time.sleep()
