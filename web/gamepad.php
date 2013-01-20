<?php require('login_xv.php'); ?>
<?php include 'menu.php'; ?>
<html>
<head>
    <title>Web interface for Neato XV-25 - Joystick</title>
    <link rel="stylesheet" type="text/css" href="style.css">
    <link rel="icon" href="favicon.ico" type="image/x-icon" />
</head>
<body>
<h1>Web interface for Neato XV-25</h1>

<?php showMenu(); ?>

<div id="inMiddle">
    <div class="form" id="gamepad">
	<h2>Gamepad</h2>
        <div id="gamepadInfo">... Gamepad ...</div>
    </div>
    <script language="javascript" type="text/javascript" src="js/gamepad.js"></script>
</div>

</body>
</html>
