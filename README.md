# ip_routing_helper
помощник перенаправления сетевых запросов, в зависимости от того доступен ли узел

Работает на VPS сервере, если доступна машина из приватной сети - то запускает команды на роутинг с VPS на эту машину
Если машина становится недоступна - запускает команды удаления маршрутов.

Т.е. своего рода переключатель VPS<->PrivateServer для cpp-code.ru

Да, нормальный человек бы написал это на python, а талантилвый так вообще на bash что бы не плодить лишнюю прослойку
ведь фактически именно системный интерпретатор гоняется, но в виду того что я не знаю ни python, ни темболее bash,
значительно быстрее было написать это на ++, благо есть stl и libconfig++ и все это делается немногим дольше (имхо)