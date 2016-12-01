//
//  UserProfileTableViewCell.swift
//  Exchange
//
//  Created by jay on 2015/11/27.
//  Copyright © 2015年 Kai-Chieh (Jay) Huang. All rights reserved.
//

import UIKit
import Parse

class UserProfileTableViewCell: UITableViewCell, UITextFieldDelegate {
    
    var imageName :String? {
        didSet{
            updateUI()
        }
    }
    
    var profilePicUrl: String? {
        didSet {
            updateUI()
        }
    }
    
    var textToDisplay :String? {
        didSet{
            updateUI()
        }
    }
    
    @IBOutlet weak var profilePicImageView: UIImageView! {
        didSet {
            profilePicImageView.layer.masksToBounds = true
            profilePicImageView.layer.cornerRadius = 8
        }
    }
    
    @IBOutlet weak var rowIconImageView: UIImageView!
    
    @IBOutlet weak var rowInfoTextfield: UITextField! {
        didSet {
            rowInfoTextfield.clearButtonMode = UITextFieldViewMode.WhileEditing
            rowInfoTextfield.borderStyle = UITextBorderStyle.None
            rowInfoTextfield.delegate = self
        }
    }
    
    // MARK: UITextfield Delegate
    func textFieldShouldReturn(textField: UITextField) -> Bool {
        if textField === rowInfoTextfield {
            textField.resignFirstResponder()
            rowInfoTextfield.text = textField.text
            // Store user entered informaion to database
            let user = PFUser.currentUser()
            if rowInfoTextfield.tag == 0 {
                user?.setValue(rowInfoTextfield.text, forKey: "username")
            }else if rowInfoTextfield.tag == 1 {
                user?.setValue(rowInfoTextfield.text, forKey: "email")
            }else if rowInfoTextfield.tag == 2 {
                user?.setValue(rowInfoTextfield.text, forKey: "phone")
            }
            user?.saveInBackground()
        }
        return true
    }
    
    private func updateUI() {
        if imageName != nil {
            rowIconImageView.image = UIImage(named: imageName!)
        }
        if textToDisplay != nil {
            rowInfoTextfield.text = textToDisplay
        }
        if let profileImageUrlString = profilePicUrl {
            if let profileImageUrl = NSURL(string: profileImageUrlString) {
                dispatch_async(dispatch_get_global_queue(QOS_CLASS_USER_INITIATED, 0), { [weak weakSelf = self] in
                    if let imageData = NSData(contentsOfURL:profileImageUrl)  {
                        dispatch_async(dispatch_get_main_queue(), { () -> Void in
                            weakSelf?.profilePicImageView.image = UIImage(data: imageData)
                        })
                    }
                })
            }
        }
    }
    
    override func awakeFromNib() {
        super.awakeFromNib()
        // Initialization code
    }
    /*
    override func setSelected(selected: Bool, animated: Bool) {
        super.setSelected(selected, animated: animated)

        // Configure the view for the selected state
    }
    */
}
