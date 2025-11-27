import chardet
import os

def check_file_encoding(file_path):
    try:
        with open(file_path, 'rb') as f:
            raw_data = f.read()
            result = chardet.detect(raw_data)
            encoding = result['encoding']
            confidence = result['confidence']
            print(f"文件: {file_path}")
            print(f"检测到的编码: {encoding}")
            print(f"可信度: {confidence:.2f}")
            return encoding
    except Exception as e:
        print(f"检查编码时出错: {str(e)}")
        return None

if __name__ == "__main__":
    file_path = r"c:\Users\33790\Desktop\Stardew_Valley_Cocos2d-x4.0\Classes\AppDelegate.h"
    encoding = check_file_encoding(file_path)
    print(f"编码检查完成: {encoding}")