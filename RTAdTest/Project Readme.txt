RTAdTest

This example is designed to test the AdManager and IAPManager.

It currently supports:

* Using tapjoy on Android (including using managed points, full screen and banner ads, offer wall) (You have to plug in your own tapjoy account info in the Main.java)
* Using In App Billing in iOS, Android Market, webOS. You would have to setup the app with your own app id, and your own items
to really test the IAP though.

Tested to build with:

* Windows GLES (Stuff doesn't really work though, you must run on the real platform to see real ads served or IAP work)
* Android
* WebOS
* iOS

App build for a tablet-sized screensize.

-- About webos IAP --

To properly test IAP on webos, you have to:

* be using the SDK/Test version of the OS on your TouchPad (you know it's the right one when the store is nearly empty)
* Your palm device account's email must match your developer account
* Your palm device account must have the US or Canada set as its country
* You must run the store first and setup a credit card (supposedly fake is ok, but for me it seemed to need
a real one..)
* You have to setup test IAP items in the developer portal, and modify this example to call those item names


-Seth (seth@rtsoft.com)