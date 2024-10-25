
<?php

if (!isset($loginErrorText))
{
    $loginErrorText = "";
}

$page = basename($_SERVER['REQUEST_URI']);

if (PM_SessionHandler::isLoggedIn())
{
    if ($page == "signup.php")
    {
        header("Location: index.php");
        exit();
    }
    echo
        "<div class=\"log-in-panel slide-in-right\">
                <div class=\"button-area\">
                    <a href=\"../pages/createpoll.php\">
                        <button id=\"create-poll-btn\">CREATE POLL</button>
                    </a>
                    <a href=\"../pages/pollmgmt.php\">
                    <button id=\"manage-polls-btn\">MANAGE MY POLLS</button>
                </a>
                </div>
            </div>";


} else if ($page == "signup.php") {
    echo "<div class=\"log-in-panel sign-up-panel slide-in-right\">
    <h4>
        SIGN UP
    </h4>
    <script src=\"../scripts/validator.js\"></script>
    <script src=\"../scripts/signup.js\" defer></script>

    <form method=\"POST\" enctype=\"multipart/form-data\">
        <label for=\"username\">Username:</label> <br />
        <input type=\"text\" id=\"username\" name=\"username\">
        <label for=\"email\">Email:</label> <br />
        <input type=\"text\" id=\"email\" name=\"email\">
        <label for=\"password\">Password:</label>
        <input type=\"password\" id=\"password\" name=\"password\">
        <label for=\"password-confirm\">Confirm password:</label>
        <input type=\"password\" id=\"password-confirm\" name=\"password-confirm\">
        <label for=\"icon\" id=\"icon-upload-label\">&nbsp;&nbsp;&nbsp;Upload avatar icon:</label>
        <button type=\"file\" id=\"icon-upload-btn\">
        <input name=\"icon\" id=\"image-file-input\" type=\"file\" accept=\".png, .jpg, .jpeg\">
        <img src=\"../images/default-icon.png\">
        </input>
        </button> <br />
        <div id=\"img-txt\"></div>
        <button id=\"sign-up-btn\">SIGN UP</button>
        </form>
    <div id=\"sign-up-err-text\">{$signUpMsg}</div>
    </div>";
} else {
    if (isset($_POST) && !PM_SessionHandler::isLoggedIn()) {
        $loginErrorText = PM_SessionHandler::handleLogin();
    }
    echo "
    <script src=\"../scripts/login.js\" defer></script>
    <script src=\"../scripts/validator.js\"></script>
    <div class=\"log-in-panel slide-in-right\">
    <h4>
        Log In
    </h4>
    <form method=\"post\">
        <label for=\"email\">Email:</label>
        <input type=\"text\" id=\"email\" name=\"email\">
        <label for=\"password\">Password:</label>
        <input type=\"password\" id=\"password\" name=\"password\">
        <a href=\"pollmgmt.php\">
            <button id=\"sign-in-btn\">LOG IN</button>
        </a>
        </form>
        <div style=\"color: \"red\";\" id=\"login-err-text\">{$loginErrorText}</div>
    </div>";
}

?>