#!/usr/bin/env python3
"""
Python 3 compatibility patch for Cocos2d-x download-deps.py
This script modifies the download-deps.py to work with Python 3
"""

import os
import re
import sys

def patch_download_deps():
    """Apply Python 3 compatibility patches to download-deps.py"""
    
    download_deps_path = os.path.join('cocos2d', 'download-deps.py')
    
    if not os.path.exists(download_deps_path):
        print(f"Error: {download_deps_path} not found!")
        sys.exit(1)
    
    # Read the original file
    with open(download_deps_path, 'r', encoding='utf-8') as f:
        content = f.read()
    
    print("==> Applying Python 3 compatibility patches...")
    
    # Patch 1: Fix Python version check
    content = re.sub(r'if major_ver > 2:', 'if major_ver > 3:', content)
    
    # Patch 2: Fix urllib2 imports and usage
    # Replace urllib2 imports with urllib.request and urllib.error
    content = re.sub(r'import urllib2', 'import urllib.request as urllib2', content)
    
    # Patch 3: Fix urllib2.HTTPError to urllib.error.HTTPError
    content = re.sub(r'urllib2\.HTTPError', 'urllib.error.HTTPError', content)
    
    # Patch 4: Add urllib.error import where needed
    # Find lines with urllib2.HTTPError and ensure urllib.error is imported
    if 'urllib.error.HTTPError' in content and 'import urllib.error' not in content:
        # Add urllib.error import after urllib.request import
        content = re.sub(
            r'(import urllib\.request as urllib2)',
            r'\1\n        import urllib.error',
            content
        )
    
    # Patch 5: Fix print statements (though most seem to be function calls already)
    # This is precautionary in case there are any print statements without parentheses
    
    # Write the patched file
    with open(download_deps_path, 'w', encoding='utf-8') as f:
        f.write(content)
    
    print("==> Python 3 compatibility patches applied successfully!")

if __name__ == '__main__':
    patch_download_deps()
