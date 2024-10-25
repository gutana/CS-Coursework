<?php require("../universalHeader.php"); ?>

<?php

$signUpMsg = "";

if ($_SERVER['REQUEST_METHOD'] == 'POST')
    {
        $username = filter_input(INPUT_POST, 'username', FILTER_SANITIZE_SPECIAL_CHARS);
        $email = filter_input(INPUT_POST, 'email', FILTER_SANITIZE_EMAIL);
        $password = filter_input(INPUT_POST, 'password', FILTER_SANITIZE_SPECIAL_CHARS);
        $pwdconfm = filter_input(INPUT_POST, 'password-confirm', FILTER_SANITIZE_SPECIAL_CHARS);

        $email = strtolower($email);

        $emailValid = InputValidator::validateEmail($email);
        $pwdValid = InputValidator::validatePassword($password);
        $pwdLenValid = InputValidator::validateMinLength($password, 8);
        $usernameValid = InputValidator::validateUsername($username);
        $usrLenValid = InputValidator::validateMaxLength($username, 40);
        $emailLenValid = InputValidator::validateMaxLength($email, 50);

        $passwordGood = false; 
        $passwordsMatch = strcmp($password, $pwdconfm); 
        
        if ($passwordsMatch == 0) {
            $passwordGood = true; 
        }
        
        $tmpFileLocation = $_FILES['icon']['tmp_name'];
        
        $inputValid = $emailValid && 
                      $pwdValid && 
                      $pwdLenValid &&
                      $usrLenValid && 
                      $usernameValid && 
                      $emailLenValid &&
                      $passwordGood;
          
        if ($inputValid)
        {
            Database::Init();
            $resp = Database::Query("SELECT * FROM users 
                                    WHERE email = \"$email\"");
            if ($resp->getRow())
            {
                $inputValid = false; 
                $signUpMsg = "Account with this email already exists.";
            } else {
                $info = pathinfo($_FILES['icon']['full_path']);
                if ($info)
                {
                    if (isset($info['extension']))
                    {
                        $extension = $info['extension'];
                        Database::RegisterUser($username, $email, $tmpFileLocation, $extension, $password);
                    } else {
                        $signUpMsg = "Image must be uploaded.";
                        $inputValid = false; 
                    }
                }
            }
            Database::Close();

        } else {
            die("Something went wrong validating sign up information.");
        }

        if ($inputValid) {
            Database::Init();
            $resp = Database::Query("SELECT user_id, username FROM users 
                                     WHERE email = \"$email\"");
            
            $data = $resp->getRow();
            print_r($data);
            

            PM_SessionHandler::logUserIn($data['username'], $data['user_id']);
        }
    }
?>

<body>
    <div class="container">
        <span class="header-left">
            <a href="index.php">
                <img src="../images/logo.png" alt="PollMaster Logo" href="index.php" />
            </a>
        </span>
        <span class="header-middle">
            <?php require("../components/toppanel.php"); ?>
        </span>
        <span class="header-right">
        <?php require("../components/headerright.php"); ?>
        </span>

        <div class="main-body-left">
            <?php require("../components/leftpanel.php"); ?>
        </div>

        <div class="main-body-middle">
            <?php include("../components/homePageText.php"); ?>
        </div>
        <div class="main-body-right">            
            <?php include("../components/recentPollsSidebar.php"); ?>
        </div>

    </div>
    <footer>
        <a href="../../index.html">
            Return to Mohammad's page
        </a>
    </footer>
</body>

</html>