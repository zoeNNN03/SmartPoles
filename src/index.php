<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Data from IOT</title>
</head>
<body>
    <?php
        $servername = "localhost";
        $username = "s65160282";
        $password = "r4ur37pZ";
        // $dbname = "SmartPoles";

        $conn = new mysqli($servername, $username, $password);
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        }
        else { echo "Connected to mysql database. <br>"; }
    ?>
</body>
</html>
