from mil import myacc

if __name__ == '__main__':
    fp = open('../../c/source/system/os/osek/bcc1/non_preempt/example.mil','r')
    data = fp.read()
    fp.close()
    print ' >> MCC example.mil'
    myacc.parse(data)
    
