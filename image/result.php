<?php

// Set the Content Type
header('Content-type: image/jpeg');
Header('Cache-Control: no-store, no-cache, must-revalidate, post-check=0, pre-check=0');
Header('Expires: Thu, 19 Nov 1981 08:52:00 GMT');
Header('Pragma: no-cache');

$result = '';
if (isset($_GET['result'])) {
  $result = $_GET['result'];
}

if ($result >= 80) {
  $stars = '&#xe9d9; &#xe9d9; &#xe9d9;';
}
elseif ($result <= 79 && $result >= 50) {
  $stars = '&#xe9d9; &#xe9d9; &#xe9d7;';
}
else {
  $stars = '&#xe9d9; &#xe9d7; &#xe9d7;';
}


// Create a 300x300 image.
$image = imagecreate(300, 300);
// Background color #ffffff.
$background = imagecolorallocate($image, 255, 255, 255);
// Text color #0072b9.
$text_colour = imagecolorallocate($image, 0, 114, 185);
// Path to the font file.
$opensans_bold = getcwd() . '/fonts/Open_Sans/OpenSans-Bold.ttf';
// Path to the icon font file.
$icomoon = getcwd() . '/fonts/Stars/icomoon.ttf';
// Text to display.
$text = $result . '%';
// Font size.
$font_size = 60;
$star_size = 30;

// Create a bounding box for the text.
$textbox = imagettfbbox($font_size, 0, $opensans_bold, $text);
$starbox = imagettfbbox($star_size, 0, $icomoon, $stars);

// Get image Width and Height
$image_width = imagesx($image);

// Text width and heigth.
$text_width = $textbox[2] - $textbox[0];
$star_width = $starbox[2] - $starbox[0];

// Calculate coordinates of the text
$tx = ($image_width / 2) - ($text_width / 2);
$sx = ($image_width / 2) - ($star_width / 2);

imagettftext($image, $font_size, 0, $tx, 200, $text_colour, $opensans_bold, $text);
imagettftext($image, $star_size, 0, $sx, 120, $text_colour, $icomoon, $stars);

imagepng($image);
imagedestroy($image);
