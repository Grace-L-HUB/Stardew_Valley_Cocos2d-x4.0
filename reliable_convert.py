import os
import sys

def is_gb2312_compatible(text):
    """检查文本是否完全兼容GB2312编码"""
    try:
        text.encode('gb2312')
        return True
    except UnicodeEncodeError:
        return False

def find_incompatible_chars(text):
    """找出文本中不兼容GB2312的字符"""
    incompatible_chars = []
    for i, char in enumerate(text):
        try:
            char.encode('gb2312')
        except UnicodeEncodeError:
            incompatible_chars.append((i, char, repr(char)))
    return incompatible_chars

def convert_to_gb2312_reliable(file_path):
    """可靠地将文件转换为GB2312编码"""
    backup_path = file_path + '.bak'
    
    try:
        # 1. 读取文件内容
        print(f"正在读取文件: {file_path}")
        
        # 尝试多种编码读取
        encodings = ['utf-8', 'gbk', 'latin-1']
        content = None
        used_encoding = None
        
        for encoding in encodings:
            try:
                with open(file_path, 'r', encoding=encoding) as f:
                    content = f.read()
                used_encoding = encoding
                print(f"✓ 成功以 {encoding} 编码读取文件")
                break
            except UnicodeDecodeError:
                print(f"✗ 无法以 {encoding} 编码读取文件")
        
        if content is None:
            print(f"❌ 无法以任何支持的编码读取文件")
            return False
        
        # 2. 备份原文件
        with open(backup_path, 'wb') as f:
            f.write(content.encode(used_encoding))
        print(f"✓ 已创建备份文件: {backup_path}")
        
        # 3. 检查GB2312兼容性
        if not is_gb2312_compatible(content):
            print("⚠ 文件包含GB2312不支持的字符，需要进行处理")
            incompatible_chars = find_incompatible_chars(content)
            print(f"发现 {len(incompatible_chars)} 个不兼容字符:")
            for i, char, repr_char in incompatible_chars[:10]:  # 只显示前10个
                print(f"  位置 {i}: {repr_char}")
            if len(incompatible_chars) > 10:
                print(f"  ... 以及其他 {len(incompatible_chars) - 10} 个字符")
            
            # 4. 处理不兼容字符
            processed_content = ''
            for char in content:
                try:
                    char.encode('gb2312')
                    processed_content += char
                except UnicodeEncodeError:
                    processed_content += '?'  # 替换为问号
            
            print(f"✓ 已处理不兼容字符，替换为问号")
        else:
            processed_content = content
            print("✓ 文件内容完全兼容GB2312编码")
        
        # 5. 以二进制模式写入GB2312编码内容
        try:
            gb2312_bytes = processed_content.encode('gb2312')
            with open(file_path, 'wb') as f:
                f.write(gb2312_bytes)
            print(f"✓ 成功以GB2312编码写入文件")
            
            # 6. 验证写入是否成功
            with open(file_path, 'rb') as f:
                test_bytes = f.read()
            
            # 比较写入和读取的字节是否一致
            if test_bytes == gb2312_bytes:
                print("✓ 文件写入验证成功，字节内容完全一致")
            else:
                print("⚠ 文件写入验证警告，字节数不一致")
                print(f"  写入: {len(gb2312_bytes)} 字节")
                print(f"  读取: {len(test_bytes)} 字节")
            
            # 7. 尝试以GB2312读取验证
            try:
                with open(file_path, 'r', encoding='gb2312') as f:
                    test_content = f.read()
                print("✓ 验证成功: 文件可以正确以GB2312编码读取")
                print(f"  读取的字符数: {len(test_content)}")
                
                # 显示文件开头部分作为样本
                sample = test_content[:100].replace('\n', '\\n')
                print(f"  文件开头样本: {sample}...")
                
                return True
                
            except Exception as e:
                print(f"❌ 验证失败: 无法以GB2312编码读取文件 - {str(e)}")
                # 恢复原文件
                if os.path.exists(backup_path):
                    with open(backup_path, 'rb') as f:
                        with open(file_path, 'wb') as f_out:
                            f_out.write(f.read())
                    print(f"✓ 已恢复原文件")
                return False
                
        except Exception as e:
            print(f"❌ 写入文件时出错: {str(e)}")
            # 恢复原文件
            if os.path.exists(backup_path):
                with open(backup_path, 'rb') as f:
                    with open(file_path, 'wb') as f_out:
                        f_out.write(f.read())
                print(f"✓ 已恢复原文件")
            return False
            
    except Exception as e:
        print(f"❌ 转换过程中发生错误: {str(e)}")
        # 尝试恢复原文件
        if os.path.exists(backup_path):
            try:
                with open(backup_path, 'rb') as f:
                    with open(file_path, 'wb') as f_out:
                        f_out.write(f.read())
                print(f"✓ 已恢复原文件")
            except:
                print("❌ 恢复原文件失败")
        return False

if __name__ == "__main__":
    file_path = r"c:\Users\33790\Desktop\Stardew_Valley_Cocos2d-x4.0\Classes\AppDelegate.h"
    print("===== GB2312编码转换工具 =====")
    print(f"目标文件: {file_path}")
    print()
    
    success = convert_to_gb2312_reliable(file_path)
    
    print()
    print(f"转换{'成功' if success else '失败'}")
    print("==============================")