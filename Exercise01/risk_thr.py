#Risk.py: Simulate Risk battles
#Rules are at http://www.hasbro.com/common/instruct/risk.pdf
import sys
import random
import time
import statistics as stats
import threading
import queue

def roll(attack_dice, defend_dice):
    '''Carry out a roll for both sides
       Return a tuple (attack_loss, defend_loss)'''
    attack_roll = []                        #Accumulates rolls of attacker's dice
    for i in range(0, attack_dice):         #Carry out attacker's rolls
        attack_roll.append(random.randint(1, 6))
    defend_roll = []                        #Accumulates rolls of defender's dice
    for i in range(0, defend_dice):         #Carry out defender's rolls
        defend_roll.append(random.randint(1, 6))
    attack_roll.sort()
    attack_roll.reverse()                   #Put attacker's rolls in descending order
    defend_roll.sort()
    defend_roll.reverse()                   #Put defender's rolls in descending order
    attack_loss = 0                         #This will count number of losees for attacker
    defend_loss = 0                         #Number of losses for defender
    for i in range(0, len(defend_roll)):    #Compare attacker's rolls with defender's
        if attack_roll[i] > defend_roll[i]: #Defender loses if attacker's is higher
            defend_loss = defend_loss + 1
        else:
            attack_loss = attack_loss + 1   #Else attacker loses (defender wins ties)
    #print(attack_roll, defend_roll, attack_loss, defend_loss)
    return attack_loss, defend_loss         #Return a tuple with number losses for each player

def run_simulations(que, num_simulations, num_armies, attack_dice, defend_dice):
    results = 0
    #for i in range(num_simulations):
    attacknum = num_armies
    defendnum = num_armies
    while attacknum > 1 and defendnum > 0: #Attacker needs at least 2 to continue
        if attacknum > 3:               #Check number of attacking armies
            aroll = attack_dice         #if > 3, roll all of attacker's dice
        else:
            aroll = 2                   #Else attacker rolls 2 dice
        if defendnum > 1:               #Check number of defending armies
            droll = defend_dice         #if > 1, roll all of defender's dice (max is 2)
        else:
            droll = 1                   #Else defender can only roll 1 die
        result = roll(aroll, droll)
        #Update number of armies for each player
        attacknum = attacknum - result[0]
        defendnum = defendnum - result[1]
    #print("Attackers:", attacknum, "Defenders:", defendnum)
    #Append number of attacker's armies at end of each simulation
    #Assume defender has 0 armies (if attacker has 1 then defender won)
    #results.append(attacknum)
    results = attacknum
    que.put(results)

def main(num_simulations):
    num_armies = 10000  #Number of armies for both attacker and defender at outset
    num_attack_dice = 3 #Number of dice for attacker when holding enough armies
    num_defend_dice = 2 #Number of dice for defender when holding enough armies

    rque = queue.Queue()
    threads = []
    results =[]
    thrds = queue.Queue()
    
    for i in range(num_simulations):
        #Carry out simulations
        t = threading.Thread(target=run_simulations, args=[rque, num_simulations, num_armies,
                              num_attack_dice, num_defend_dice], name="t"+str(i))
        t.start()
        threads.append(t)
        
    for j in range(len(threads)):
        #threads[j].join()
        thrds.put(threads[j])
        results.append(rque.get())
    thrds.join()
    print("R: ", results)               #print check
    #Print statistics
    print(threading.currentThread().getName(), " MEAN: ", round(stats.mean(results), 2),
          "STD DEV: ", round(stats.stdev(results), 2),
          "MEDIAN: ", stats.median(results))

if __name__ == "__main__":
    num_simulations = int(sys.argv[1])
    start = time.time()
    main(num_simulations)
    elapsed_time = time.time() - start
    print("Elapsed time: {}".format(elapsed_time))
