<?php 

$page = basename($_SERVER['REQUEST_URI']);

if (PM_SessionHandler::isLoggedIn())
{
    echo "<a href=\"logout.php\">
            <button name=\"sign-out\" id=\"sign-out-btn\">SIGN OUT</button>
          </a>";

} else if ($page == "signup.php") {
    echo "<a href=\"index.php\">
            <button id=\"sign-in-btn\">LOG IN</button>
          </a>";
} else {
    echo "<a href=\"signup.php\">
            <button id=\"sign-up-btn\">SIGN UP</button>
    </a>";
}