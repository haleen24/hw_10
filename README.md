### В рамках решения представлены 2 файла: сервер и клиент
## Для запуска сервера передайте ему порт, он будет поднят по ip адресу 0.0.0.0
## Для запуска клиента передайте ему адрес сервера и порт сервера
Принцип работы: 
Первым поднимается сервер. Если поднять клиенты без сервера, они не смогут получить коннект. После запускаются 2 клиента. Первый запущенный клиент назначается отправителем, второй - слушателем. Роли назначает сервер - кто первый постучался на сервер, тот получит роль.
Переданный текст так же отображается на сервере.
