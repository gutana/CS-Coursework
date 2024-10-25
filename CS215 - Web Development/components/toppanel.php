<?php 

$text = "";
$imgSrc = "../images/default-icon.png";

if (PM_SessionHandler::isLoggedIn())
{
    $user = $_SESSION['username'];
    $text = "User: " . $user;
    
    $userID =  $_SESSION['uid'];
    Database::Init();
    $resp = Database::Query("SELECT img_path FROM users 
                               WHERE user_id = \"{$userID}\";");
    Database::Close();
    
    $path = $resp->getRow();
    if ($path) {
        $imgSrc = "../uploads/{$path['img_path']}";
    }

}
else 
{
    $text = "Browsing as guest.";
}



echo
"<div class=\"guest-header-middle\">
<div>
    <img src=\"{$imgSrc}\"\>
    {$text}
</div>
</div>";

?>
