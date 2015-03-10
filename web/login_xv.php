<?php
$password = '813862ac0d98bf89b79376dd314c71932af539c2';

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
    <script language="javascript" type="text/javascript">
     window.onload = function() {
            var pwd = document.getElementById('password');
            pwd.focus();
            pwd.select();
     }
    </script>
</head>
<body>

    <div id ="inMiddle" class="error_404">
        <p style="text-align: center;">
            <img class="centered" src="img/bigLogo.png" alt="neato XV-25" style="margin-top:20px;" />
        </p>

        <div class="form">
            <h2>You need to login !</h2>
            <form method="post">
                <input id="password" type="password" name="password">
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
