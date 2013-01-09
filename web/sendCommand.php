
<?php
error_reporting(E_ALL);

if (isset($_POST['cmd'])) {

    $ip = $_COOKIE['ipCookie'];
    $port = $_COOKIE['portCookie'];

    $service_port = $port;
    $address = gethostbyname($ip);
    $error = 0;

    if (!($socket = socket_create(AF_INET, SOCK_STREAM, 0)))
        $error = 1;

    if (0 == $error) {
        if (!(socket_connect($socket, $address, $service_port)))
            $error = 1;
    }

    if (0 == $error) {
        $in = htmlspecialchars($_POST['cmd']) . "\n";
        if (socket_write($socket ,$in ,strlen($in)) === false)
            $error = 1;
    }

    if (0 == $error) {
        $out = "";
        $response = "";
        $receivedDone = 0;
        while (0 == $receivedDone && 0 == $error) {
            if (false === socket_recv($socket, $out, 1024, MSG_WAITALL)) {
                $error = 1;
            } else {
                $response .= $out;
                if (false !== ($endOfResponse = strpos($response, ",EndOfResponse"))) {
                    $receivedDone = 1;
                    $response = substr($response, 0, $endOfResponse);
                    $response = str_replace("~", "<br/>", $response);
                }
            }
        }
    }

    echo $response;
    socket_close($socket);
}
?>
