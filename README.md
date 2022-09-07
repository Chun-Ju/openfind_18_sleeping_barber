# openfind_18_sleeping_barber
sleeping barber, practice about IPC and multithread.</br>
</br>
## EXECUTION:
#### **(Terminal 1 for command.c)**</br>
> make                                       </br>
> ./c                                        </br>
#### **(Terminal 1 for barberShop.c)**</br>
> ./b                                        </br>

## USER INPUT(in command) AND CORRESPONDING OUTPUT(in barberShop)

### display test data 1(real order and output in barbershop might not the same everytime(will change with different input timing), it's just an example)
#### **(Terminal 1)**</br>
> ------------------------------------------ </br>
> PLEASE ENTER a OR c THEN ENTER ONLY        </br>
> command 'a': means customers arrive        </br>
> command 'c': means time to close the store </br>
> ------------------------------------------ </br>
> Command: a                                 </br>
> Command: a                                 </br>
> Command: a                                 </br>
> Command: a                                 </br>
> Command: a                                 </br>
> Command: a                                 </br>
> Command: a                                 </br>
> Command: a                                 </br>
> Command: a                                 </br>
> Command: a                                 </br>
> Command: a                                 </br>
> Command: a                                 </br>
> Command: a                                 </br>
> Command: a                                 </br>
> Command: a                                 </br>
> Command: a                                 </br>
> Command:                                   </br>
> COMMAND CANNOT BE EMPTY!!                  </br>
> Command: a                                 </br>
> Command: aa                                </br>
> COMMAND IS TOOOO LONG!!                    </br>
> Command: a                                 </br>
> Command: a                                 </br>
> Command: a                                 </br>
> Command: a                                 </br>
> Command: a                                 </br>
> Command: a                                 
> Command: c                                 </br>

#### **(Terminal 2)**</br>

> barber1 opens the door                     </br>
> barber2 opens the door                     </br>
> barber3 opens the door                     </br>
> customer1 arrives the store                </br>
> customer1 waiting in the bench             </br>
> barber1 is serving customer1               </br>
> customer2 arrives the store                </br>
> customer2 waiting in the bench             </br>
> barber2 is serving customer2               </br>
> customer3 arrives the store                </br>
> customer3 waiting in the bench             </br>
> barber3 is serving customer3               </br>
> customer4 arrives the store                </br>
> customer4 waiting in the bench             </br>
> customer5 arrives the store                </br>
> customer5 waiting in the bench             </br>
> customer6 arrives the store                </br>
> customer6 waiting in the bench             </br>
> customer7 arrives the store                </br>
> customer7 waiting in the bench             </br>
> customer8 arrives the store                </br>
> customer8 waiting in the bench             </br>
> customer9 arrives the store                </br>
> customer9 waiting in the bench             </br>
> barber1 has finished serving customer1, so customer1 leaves. </br>
> barber1 is serving customer4               </br>
> customer10 arrives the store               </br>
> customer10 waiting in the bench            </br>
> barber2 has finished serving customer2, so customer2 leaves. </br>
> barber2 is serving customer5               </br>
> customer11 arrives the store               </br>
> customer11 waiting in the bench            </br>
> barber3 has finished serving customer3, so customer3 leaves. </br>
> barber3 is serving customer6               </br>
> customer12 arrives the store               </br>
> customer12 waiting in the bench            </br>
> customer13 arrives the store               </br>
> customer13 waiting in the bench            </br>
> customer14 arrives the store               </br>
> customer14 waiting in the bench            </br>
> customer15 arrives the store               </br>
> customer15 waiting in the bench            </br>
> customer16 arrives the store               </br>
> customer16 waiting in the bench            </br>
> customer17 arrives the store               </br>
> customer17 leaves due to no seats in waiting room            </br>
> barber1 has finished serving customer4, so customer4 leaves. </br>
> barber1 is serving customer7               </br>
> barber2 has finished serving customer5, so customer5 leaves. </br>
> barber2 is serving customer8               </br>
> customer18 arrives the store               </br>
> customer18 waiting in the bench            </br>
> barber3 has finished serving customer6, so customer6 leaves. </br>
> barber3 is serving customer9               </br>
> customer19 arrives the store               </br>
> customer19 waiting in the bench            </br>
> customer20 arrives the store               </br>
> customer20 waiting in the bench            </br>
> customer21 arrives the store               </br>
> customer21 leaves due to no seats in waiting room            </br>
> customer22 arrives the store               </br>
> customer22 leaves due to no seats in waiting room            </br>
> ------------------------------             </br>
> |  ready to close the store  |             </br>
> ------------------------------             </br>
> dismiss customer 10,11,12,13,14,15,16,18,19,20 in the bench. </br>
> barber1 has finished serving customer7, so customer7 leaves. </br>
> barber1 sleeps                             </br>
> barber2 has finished serving customer8, so customer8 leaves. </br>
> barber2 sleeps                             </br>
> barber3 has finished serving customer9, so customer9 leaves. </br>
> barber3 sleeps                             </br>
> close                                      </br>

