<?php
        $servername = "localhost";
        $username = "s65160282";
        $password = "r4ur37pZ";
        $dbname = "s65160282";

        $conn = new mysqli($servername, $username, $password, $dbname);
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        }
        else { echo "Connected to mysql database. <br>"; }
?>