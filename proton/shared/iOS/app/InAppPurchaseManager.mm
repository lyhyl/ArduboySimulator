//
//  InAppPurchaseManager.m
//  RTAdTest
//
//  Created by Seth Robinson on 3/15/12.
//  Copyright (c) 2012 Robinson Technologies. All rights reserved.
//

#import "InAppPurchaseManager.h"
#include "Manager/IAPManager.h"

@implementation InAppPurchaseManager


 
 //we'd only need this stuff to scan the store and get names and prices... Not implemented yet! 
- (void)GetProductData: (string)itemID
{
    NSString *str =  [NSString stringWithCString: itemID.c_str() encoding: [NSString defaultCStringEncoding]];

    LogMsg("Requesting product data for %s", itemID.c_str());
    NSSet *productIdentifiers = [NSSet setWithObject:str ];
    productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:productIdentifiers];
    productsRequest.delegate = self;
    [productsRequest start];
    
    // we will release the request object in the delegate callback
}



- (void)request:(SKRequest *)request didFailWithError:(NSError *)error
{
    LogMsg("From iOS> We have a error in the IAP request");
}


- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    NSArray *products = response.products;
    proUpgradeProduct = [products count] == 1 ? [[products firstObject] retain] : nil;
    if (proUpgradeProduct)
    {
        NSLog(@"Product title: %@" , proUpgradeProduct.localizedTitle);
        NSLog(@"Product description: %@" , proUpgradeProduct.localizedDescription);
        NSLog(@"Product price: %@" , proUpgradeProduct.price);
        NSLog(@"Product id: %@" , proUpgradeProduct.productIdentifier);
    }
    
    for (NSString *invalidProductId in response.invalidProductIdentifiers)
    {
        NSLog(@"Invalid product id: %@" , invalidProductId);
    }
    
    // finally release the reqest we alloc/init’ed in requestProUpgradeProductData
    [productsRequest release];
    
    [[NSNotificationCenter defaultCenter] postNotificationName:kInAppPurchaseManagerProductsFetchedNotification object:self userInfo:nil];
} 


//
// call this method once on startup
//
- (void)InitIAP
{
    // restarts any purchases if they were interrupted last time the app was open
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
}

//
// call this before making a purchase
//
- (BOOL)canMakePurchases
{
    return [SKPaymentQueue canMakePayments];
}

//
// kick off the upgrade transaction
//
- (void) BuyItemByID:(string)itemID
{
    
    if ([self canMakePurchases] == false)
    {
        
        GetMessageManager()->SendGUIStringEx(MESSAGE_TYPE_IAP_RESULT,(float)IAPManager::RESULT_SERVICE_UNAVAILABLE,0.0f,0, "Service unavailable");
        return;
    }
    NSString *str =  [NSString stringWithCString: itemID.c_str() encoding: [NSString defaultCStringEncoding]];

    SKPayment *payment = [SKPayment paymentWithProductIdentifier:str];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}
 
//
// called when the transaction was successful
//
- (void)completeTransaction:(SKPaymentTransaction *)transaction
{
    LogMsg("Transaction complete");
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
       GetMessageManager()->SendGUIStringEx(MESSAGE_TYPE_IAP_RESULT,(float)IAPManager::RESULT_OK,0.0f,0, "Complete");
}

//
// called when a transaction has been restored and and successfully completed
//
- (void)restoreTransaction:(SKPaymentTransaction *)transaction
{
    LogMsg("Restoring transaction");
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
    GetMessageManager()->SendGUIStringEx(MESSAGE_TYPE_IAP_RESULT,(float)IAPManager::RESULT_OK_ALREADY_PURCHASED,0.0f,0, "Restore");
}

//
// called when a transaction has failed
//
- (void)failedTransaction:(SKPaymentTransaction *)transaction
{
    if (transaction.error.code != SKErrorPaymentCancelled)
    {
        LogMsg("Transaction failed");
        // error!
        GetMessageManager()->SendGUIStringEx(MESSAGE_TYPE_IAP_RESULT,(float)IAPManager::RESULT_ERROR,0.0f,0, "Error");
    }
    else
    {
        GetMessageManager()->SendGUIStringEx(MESSAGE_TYPE_IAP_RESULT,(float)IAPManager::RESULT_USER_CANCELED,0.0f,0, "Canceled");
        LogMsg("Transaction failed, user canceled");
    }

    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];

}

//
// called when the transaction status is updated
//
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for (SKPaymentTransaction *transaction in transactions)
    {
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased:
                [self completeTransaction:transaction];
                break;
            case SKPaymentTransactionStateFailed:
                [self failedTransaction:transaction];
                break;
            case SKPaymentTransactionStateRestored:
                [self restoreTransaction:transaction];
                break;
            default:
                break;
        }
    }
} 

@end
