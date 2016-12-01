//
//  SearchResultCellTableViewCell.swift
//  Exchange
//
//  Created by jay on 2015/12/2.
//  Copyright © 2015年 Kai-Chieh (Jay) Huang. All rights reserved.
//

import UIKit

class SearchResultCellTableViewCell: UITableViewCell {

    var profilePicUrl: String? {
        didSet {
            updateUI()
        }
    }

    var typeName: String? {
        didSet {
            updateUI()
        }
    }
    
    var exchangeItemTitle: String? {
        didSet {
            updateUI()
        }
    }
    
    var exchangeItemDescription: String? {
        didSet {
            updateUI()
        }
    }
    
    @IBOutlet weak var userImageView: UIImageView! {
        didSet {
            userImageView.layer.masksToBounds = true
            userImageView.layer.cornerRadius = 8
        }
    }
    
    @IBOutlet weak var userNameLabel: UILabel!
    
    @IBOutlet weak var exchangeItemTitleLabel: UILabel!
    
    @IBOutlet weak var exchangeItemDescriptionLabel: UILabel!
    
    
    private func updateUI() {
        if let profileImageUrlString = profilePicUrl {
            if let profileImageUrl = NSURL(string: profileImageUrlString) {
                dispatch_async(dispatch_get_global_queue(QOS_CLASS_USER_INITIATED, 0), { [weak weakSelf = self] in
                    if let imageData = NSData(contentsOfURL:profileImageUrl)  {
                        dispatch_async(dispatch_get_main_queue(), { () -> Void in
                            weakSelf?.userImageView.image = UIImage(data: imageData)
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

    override func setSelected(selected: Bool, animated: Bool) {
        super.setSelected(selected, animated: animated)

        // Configure the view for the selected state
    }

}
