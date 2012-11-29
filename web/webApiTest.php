<?php
    error_reporting(E_ALL);

    echo "<h2>Web API Test</h2>\n";

    /* Lit le port du service WWW. */
    $service_port = getservbyname('www', 'tcp');

    /* Lit l'adresse IP du serveur de destination */
    $address = gethostbyname('127.0.0.1');

    /* Cr�e un socket TCP/IP. */
    $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
    if ($socket === false) {
        echo "socket_create() a �chou� : raison :  " . socket_strerror(socket_last_error()) . "\n";
    } else {
        echo "OK.\n";
    }

    echo "Essai de connexion � '$address' sur le port '$service_port'...";
    $result = socket_connect($socket, $address, $service_port);
    if ($socket === false) {
        echo "socket_connect() a �chou� : raison : ($result) " . socket_strerror(socket_last_error($socket)) . "\n";
    } else {
        echo "OK.\n";
    }

    $in = "HEAD / HTTP/1.0\r\n\r\n";
    $in .= "Host: www.example.com\r\n";
    $in .= "Connection: Close\r\n\r\n";
    $out = '';

    echo "Envoi de la requ�te HTTP HEAD...";
    socket_write($socket, $in, strlen($in));
    echo "OK.\n";

    echo "Lire la r�ponse : \n\n";
    while ($out = socket_read($socket, 2048)) {
        echo $out;
    }

    echo "Fermeture du socket...";
    socket_close($socket);
    echo "OK.\n\n";
?>