//
//  UserProfileTableViewController.swift
//  Exchange
//
//  Created by jay on 2015/11/14.
//  Copyright © 2015年 Kai-Chieh (Jay) Huang. All rights reserved.
//
//-------------------------------------------------------------------//
//----User Schema----------------------------------------------------//
//1. Name
//2. Email
//3. Phone Number
//4. Picture
//5. Position
//6. Provision (The skills or items this user can provide)
//7. Need      (The skills or items this user needs)
//-------------------------------------------------------------------//
//----References-----------------------------------------------------//
//[1] Parse Documentation : https://parse.com/docs/ios/guide#users-properties
//[2]
//[3] Rround Corner ImageView: http://atastypixel.com/blog/easy-rounded-corners-on-uitableviewcell-image-view/
//[4] Icons: https://icons8.com/web-app/for/ios7/email
//[5] Allow Download from http: http://ste.vn/2015/06/10/configuring-app-transport-security-ios-9-osx-10-11/
//[6] Easy way to do popoverview: http://richardallen.me/2014/11/28/popovers.html
//----Issues-----------------------------------//
//1. When change user this view will capture some information of previous user
//----To Do List-------------------------------//
//4. User location
//5. $$ Blocking main thread 
//---------------------------------------------//

import UIKit
import Parse

class UserProfileTableViewController: UITableViewController, UIPopoverPresentationControllerDelegate {
    
    struct Constants {
        static let cellHieghtForUserProfile :CGFloat = 80
        static let cellHieghtNormal :CGFloat = 50
    }
    
    enum Sections : Int {
        case UserInfo = 0
        case Provision = 1
        case Need = 2
    }
    
    private var user : PFUser?  {
        get{
            return PFUser.currentUser()
        }
    }
    
    @IBAction func addExchangeObject(sender: AnyObject) {
        self.performSegueWithIdentifier("showProvisionDetail", sender: nil)
    }
    
    //MARK: ViewController Life Cycle
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // One liner to resign keyboard when tapped outside
        view.addGestureRecognizer(UITapGestureRecognizer(target: view, action: "endEditing:"))
        title = "Profile"
        navigationController?.navigationBar.titleTextAttributes = [NSForegroundColorAttributeName : UIColor.whiteColor()]
        navigationController?.navigationBar.barTintColor = UIColor.purpleColor()
        
        // Uncomment the following line to preserve selection between presentations
        // self.clearsSelectionOnViewWillAppear = false

        // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
        // self.navigationItem.rightBarButtonItem = self.editButtonItem()
    }

    override func viewWillAppear(animated: Bool) {
        super.viewWillAppear(animated)
        tableView.reloadData()
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    // MARK: - Table view data source

    override func numberOfSectionsInTableView(tableView: UITableView) -> Int {
        // #warning Incomplete implementation, return the number of sections
        return 3
    }
    
    override func tableView(tableView: UITableView, heightForRowAtIndexPath indexPath: NSIndexPath) -> CGFloat {
        return Constants.cellHieghtNormal
    }
    
    override func tableView(tableView: UITableView, titleForHeaderInSection section: Int) -> String? {
        switch section {
        case 0: return "Personal Info"
        case 1: return "Provisions"
        case 2: return "Needs"
        default: return "Information"
        }
    }
    override func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        switch section {
        case 0: return 3
        case 1: return (user!["Provision"] != nil) ? (user!["Provision"] as! [PFObject]).count : 0
        case 2: return (user!["Need"] != nil) ? (user!["Need"] as! [PFObject]).count : 0
        default: return 2
        }
    }

    
    override func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        var cellIdentifier = ""
        if indexPath.section == Sections.UserInfo.rawValue {
            cellIdentifier = "userProfileCell"
        }else {
            cellIdentifier = "exchangeObjectCell"
        }
        let cell = tableView.dequeueReusableCellWithIdentifier(cellIdentifier, forIndexPath: indexPath)
        cell.selectionStyle = UITableViewCellSelectionStyle.None
        
        if user != nil {
            if indexPath.section == Sections.UserInfo.rawValue {
                if let userProfileCell = cell as? UserProfileTableViewCell {
                    if indexPath.row == 0{
                        userProfileCell.imageName = "nameIcon"
                        userProfileCell.rowInfoTextfield.tag = 0
                        if let name = user!.valueForKey("username") as? String{
                            userProfileCell.textToDisplay = name
                        }else {
                            userProfileCell.rowInfoTextfield.placeholder = "User Name"
                        }
                        if let profileUrl = user!.valueForKey("profilePicUrl") as? String {
                            userProfileCell.profilePicUrl = profileUrl
                        }
                    }else if indexPath.row == 1{
                        userProfileCell.imageName = "email"
                        userProfileCell.rowInfoTextfield.tag = 1
                        userProfileCell.profilePicUrl = ""
                        userProfileCell.profilePicImageView.image = nil
                        if let email = user!.valueForKey("email") as? String {
                            userProfileCell.textToDisplay = email
                        }else {
                            userProfileCell.rowInfoTextfield.placeholder = "Email"
                        }
                    }else if indexPath.row == 2{
                        userProfileCell.imageName = "phone"
                        userProfileCell.rowInfoTextfield.tag = 2
                        userProfileCell.profilePicUrl = ""
                        userProfileCell.profilePicImageView.image = nil
                        if let phone = user!.valueForKey("phone") as? String {
                            userProfileCell.textToDisplay = phone
                        }else {
                            userProfileCell.rowInfoTextfield.placeholder = "Phone Number"
                        }
                    }
                }
            }else if indexPath.section == Sections.Provision.rawValue {
                if let exchangeObjectCell = cell as? ExchangeObjectTableViewCell {
                    if let provisions = user!["Provision"] as? [PFObject] {
                        exchangeObjectCell.imageName = "provide"
                        provisions[indexPath.row].fetchIfNeededInBackgroundWithBlock({ (object, error) -> Void in
                            if error == nil {
                                if let textToDisplay = object!["Title"] as? String {
                                    exchangeObjectCell.textToDisplay = textToDisplay
                                    exchangeObjectCell.accessoryType = UITableViewCellAccessoryType.DetailButton
                                }else {
                                    exchangeObjectCell.textToDisplay = ""
                                }
                            }
                        })
                    }
                }
            }else if indexPath.section == Sections.Need.rawValue {
                if let exchangeObjectCell = cell as? ExchangeObjectTableViewCell {
                    if let needs = user!["Need"] as? [PFObject] {
                        exchangeObjectCell.imageName = "need"
                        needs[indexPath.row].fetchIfNeededInBackgroundWithBlock({ (object, error) -> Void in
                            if error == nil {
                                if let textToDisplay = object!["Title"] as? String {
                                    exchangeObjectCell.textToDisplay = textToDisplay
                                    exchangeObjectCell.accessoryType = UITableViewCellAccessoryType.DetailButton
                                }else {
                                    exchangeObjectCell.textToDisplay = ""
                                }
                            }
                        })
                    }
                }
            }
        }
        return cell
    }
    
    override func tableView(tableView: UITableView, accessoryButtonTappedForRowWithIndexPath indexPath: NSIndexPath) {
        if indexPath.section == Sections.Provision.rawValue {
            if let provisions = user!["Provision"] as? [PFObject] {
                provisions[indexPath.row].fetchIfNeededInBackground()
                self.performSegueWithIdentifier("showProvisionDetail", sender: provisions[indexPath.row])
            }
        }else if indexPath.section == Sections.Need.rawValue{
            if let needs = user!["Need"] as? [PFObject] {
                needs[indexPath.row].fetchIfNeededInBackground()
                self.performSegueWithIdentifier("showProvisionDetail", sender: needs[indexPath.row])
            }
        }
    }
    
    override func tableView(tableView: UITableView, canEditRowAtIndexPath indexPath: NSIndexPath) -> Bool {
        if (indexPath.section == Sections.Provision.rawValue) || (indexPath.section == Sections.Need.rawValue){
            return true
        }
        return false
    }
    
    override func tableView(tableView: UITableView, commitEditingStyle editingStyle: UITableViewCellEditingStyle, forRowAtIndexPath indexPath: NSIndexPath) {
        if editingStyle == .Delete {
            // User delete this exchange object 
            if indexPath.section == Sections.Provision.rawValue {
                if  var provisions = user!["Provision"] as? [PFObject] {
                    provisions[indexPath.row].deleteInBackground()
                    provisions.removeAtIndex(indexPath.row)
                    user!["Provision"] = provisions
                    
                }
            }else if indexPath.section == Sections.Need.rawValue{
                if var needs = user!["Need"] as? [PFObject] {
                    needs[indexPath.row].deleteInBackground()
                    needs.removeAtIndex(indexPath.row)
                    user!["Need"] = needs
                }
            }
            tableView.reloadData()
            user?.saveInBackground()
        }
    }
    
    // MARK: Segue
    
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        if segue.identifier == "showProvisionDetail" {
            if let showExchangeDetialMVC = segue.destinationViewController as? AddExchangeViewController {
                if let exchangeObject = sender as? PFObject {
                    showExchangeDetialMVC.exchangeObject = exchangeObject
                }
            }
        }else if segue.identifier == "popoverLogout" {
            let popoverVC = segue.destinationViewController
            popoverVC.modalPresentationStyle = .Popover
            popoverVC.popoverPresentationController!.delegate = self
        }
    }
    
    // MARK: PopoverViewController
    
    func adaptivePresentationStyleForPresentationController(controller: UIPresentationController) -> UIModalPresentationStyle {
        return UIModalPresentationStyle.None
    }
    
    /*
    // Override to support conditional editing of the table view.
    override func tableView(tableView: UITableView, canEditRowAtIndexPath indexPath: NSIndexPath) -> Bool {
        // Return false if you do not want the specified item to be editable.
        return true
    }
    */

    /*
    // Override to support editing the table view.
    override func tableView(tableView: UITableView, commitEditingStyle editingStyle: UITableViewCellEditingStyle, forRowAtIndexPath indexPath: NSIndexPath) {
        if editingStyle == .Delete {
            // Delete the row from the data source
            tableView.deleteRowsAtIndexPaths([indexPath], withRowAnimation: .Fade)
        } else if editingStyle == .Insert {
            // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
        }    
    }
    */

    /*
    // Override to support rearranging the table view.
    override func tableView(tableView: UITableView, moveRowAtIndexPath fromIndexPath: NSIndexPath, toIndexPath: NSIndexPath) {

    }
    */

    /*
    // Override to support conditional rearranging of the table view.
    override func tableView(tableView: UITableView, canMoveRowAtIndexPath indexPath: NSIndexPath) -> Bool {
        // Return false if you do not want the item to be re-orderable.
        return true
    }
    */

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */

}
