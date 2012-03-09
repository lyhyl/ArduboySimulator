RTAdTest

This example is designed to test the AdManager and IAPManager.

Created for iPad screen rez or higher.

It currently supports:

* Using tapjoy on Android (including using managed points, full screen and banner ads, offer wall)
* Using In App Billing in Android Market

Tested to build with:

* Windows GLES
* Android
* WebOS

-- About webos IAP --

To properly test IAP on webos, you have to:

* be using the SDK/Test version of the OS on your TouchPad (you know it's the right one when the store is nearly empty)
* Your palm device account's email must match your developer account
* Your palm device account must have the US or Canada set as its country
* You must run the store first and setup a credit card (supposedly fake is ok, but for me it seemed to need
a real one..)
* You have to setup test IAP items in the developer portal, and modify this example to call those item names


-Seth (seth@rtsoft.com)