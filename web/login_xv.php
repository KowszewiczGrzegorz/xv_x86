<?php
$password = '13b0600885dbce6125700fd0bd0121ed28ce3e17';

session_start();
if (!isset($_SESSION['loggedIn'])) {
    $_SESSION['loggedIn'] = false;
}

if (isset($_POST['password'])) {
    if (sha1($_POST['password']) == $password) {
        $_SESSION['loggedIn'] = true;
    } else {
        die ('Incorrect password');
    }
} 

if (!$_SESSION['loggedIn']): ?>

<html>
<head>
    <link rel="stylesheet" type="text/css" href="style.css">
    <link rel="icon" href="favicon.ico" type="image/x-icon" />
    <title>Web interface for Neato XV-25 - login</title>
</head>
<body>

    <div id ="inMiddle" class="error_404">
        <p style="text-align: center;">
            <img class="centered" src="img/bigLogo.png" alt="neato XV-25" style="margin-top:20px;" />
        </p>

        <div class="form">
            <h2>You need to login !</h2>
            <form method="post">
                <input type="password" name="password">
                <input class="moreButton historyButtons" type="submit" name="submit" value="Login">
             </form>
        </div>
    </div>
</body>
</html>

<?php
exit();
endif;
?>